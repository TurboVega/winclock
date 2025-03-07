/*
MIT License

Copyright (c) 2025 TurboVega

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "../../agwin/src/agwincore.h"
#include "../../agwin/src/agwinmsg.h"
#include <agon/vdp_vdu.h>
#include <stdio.h>

// Missing from AgDev header file
void vdp_set_rtc(int year, int month, int day, int hours, int minutes, int seconds);

extern AwAppHeader _agwin_header;
const AwFcnTable* core;

// The "my..." code and data is used for a simple test, and will
// be removed once there is a better example of a loaded app.

int32_t winclock_handle_message(AwWindow* window, AwMsg* msg, bool* halt);

AwClass winclock_class = { "winclock", NULL, winclock_handle_message };

AwApplication _agwin_app = { "winclock", 0, 0, &winclock_class, NULL, 1 };

AwRtcData rtc_data;

int main( void )
{
    core = _agwin_header.core_functions;
    winclock_class.parent = (*core->get_root_class)();

    AwWindowStyle style;
    style.border = 1;
    style.title_bar = 1;
    style.close_icon = 1;
    style.minimize_icon = 1;
    style.maximize_icon = 1;
    style.menu_icon = 1;
    style.sizeable = 1;
    style.moveable = 1;
    style.primary = 1;
    style.need_rtc = 1;

    AwWindowState state;
    state.active = 0;
    state.enabled = 1;
    state.selected = 0;
    state.visible = 1;

    AwWindow* win = (*core->create_window)(&_agwin_app, NULL, &winclock_class,
        style, state, 20, 20, 204, 214, "WinClock", 0);
    if (win) {
        win->bg_color = 14;
        win->fg_color = 0;
    }

    vdp_set_rtc(2025, 12, 31, 23, 57, 41);

	return 0;
}

/*
    The table below was produced by this code (and then cleaned up a bit):

    #include <stdio.h>
    #include <math.h>

    int main()
    {
        for (int i = 0; i < 12; i++) {
            double angle = M_PI*i/6.0;
            int mx = (int)(cos(angle-M_PI/2) * 256);
            int my = (int)(sin(angle-M_PI/2) * 256);
            printf("{ %i, %i }, // [%02i] time %02i:00, angle %f\n", mx, my, i, i, angle);
        }
        return 0;
    }

*/

const struct {
    int16_t x_mul;
    int16_t y_mul;
} multipliers[12] = {
    {    0, -256 }, // [00] time 00:00, angle 0.000000
    {  127, -221 }, // [01] time 01:00, angle 0.523599
    {  221, -128 }, // [02] time 02:00, angle 1.047198
    {  256,    0 }, // [03] time 03:00, angle 1.570796
    {  221,  127 }, // [04] time 04:00, angle 2.094395
    {  127,  221 }, // [05] time 05:00, angle 2.617994
    {    0,  256 }, // [06] time 06:00, angle 3.141593
    { -127,  221 }, // [07] time 07:00, angle 3.665191
    { -221,  128 }, // [08] time 08:00, angle 4.188790
    { -256,    0 }, // [09] time 09:00, angle 4.712389
    { -221, -128 }, // [10] time 10:00, angle 5.235988
    { -128, -221 }, // [11] time 11:00, angle 5.759587
};

typedef struct {
    uint32_t seconds:10;
    uint32_t minutes:6;
} TimeFields;

int32_t on_paint_window(AwWindow* window, AwMsg* msg, bool* halt) {
    *halt = true; // no more handling after this
    if (!window->state.visible) {
        return 0;
    }

    (*core->paint_window)(msg); // paint borders and title bar, if needed

    AwDoMsgPaintWindow* paint_msg = &msg->do_paint_window;
    AwPaintFlags* paint_flags = &paint_msg->flags;
    if (paint_flags->client || paint_flags->window) {
        (*core->set_client_viewport)(window);
        AwSize size = (*core->get_client_size)(window);
        int16_t center_x = size.width / 2;
        int16_t center_y = size.height / 2;

        vdp_set_graphics_colour(0, window->bg_color | 0x80);
        vdp_set_graphics_colour(0, 15);
        int16_t length = min(size.width, size.height);
        int16_t radius = (length * 7) / 20;
        vdp_move_to(center_x, center_y);
        vdp_plot(0x9D, center_x + radius, center_y + radius);

        vdp_set_graphics_colour(0, window->fg_color);
        vdp_move_to(center_x, center_y);
        vdp_plot(0x95, center_x + radius, center_y + radius);

        int16_t dot_radius = length / 40;
        radius = (length * 31) / 80;
        for (int16_t i = 0; i < 12; i++) {
            int16_t x_pos = center_x + (radius * multipliers[i].x_mul) / 256;
            int16_t y_pos = center_y + (radius * multipliers[i].y_mul) / 256;
            vdp_move_to(x_pos, y_pos);
            vdp_plot(0x9D, x_pos + dot_radius, y_pos + dot_radius);
        }

        vdp_move_to(center_x - 8*4, center_y - 8*2);
        vdp_write_at_graphics_cursor();
        vdp_set_graphics_colour(0, 7);

        const uint8_t* b = (const uint8_t*)&rtc_data;
        const uint32_t t = *((const uint32_t*)&rtc_data);
        uint8_t hour = (uint8_t)((t & 0x03E00000) >> 21);
        uint8_t minute = (uint8_t)((t & 0xFC000000) >> 26);
        uint8_t second = b[4];
        printf("%02hu:%02hu:%02hu", hour, minute, second);
    }

    return 0;
}

int32_t on_rtc_event(AwWindow* window, AwMsg* msg, bool* halt) {
    *halt = true; // no more handling after this
    if (window->state.visible && !window->state.minimized) {
        rtc_data.rtc_data = msg->on_real_time_clock_event.rtc.rtc_data;
        (*core->invalidate_client)(window);
    }
    return 0;
}

int32_t winclock_handle_message(AwWindow* window, AwMsg* msg, bool* halt) {
    switch (msg->do_common.msg_type) {
        case Aw_Do_PaintWindow: {
            on_paint_window(window, msg, halt);
            break;
        }

        default: {
            switch (msg->on_common.msg_type) {
                case Aw_On_RealTimeClockEvent: {
                    on_rtc_event(window, msg, halt);
                }

                default: {
                    break;
                }
            }
            break;
        }
    }
	return 0; // default to core processing
}
