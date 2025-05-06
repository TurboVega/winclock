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

    AwCreateWindowParams params;
    params.app = &_agwin_app;
    params.parent = NULL;
    params.wclass = &winclock_class;
    params.style.border = 1;
    params.style.title_bar = 1;
    params.style.close_icon = 1;
    params.style.minimize_icon = 1;
    params.style.maximize_icon = 1;
    params.style.menu_icon = 1;
    params.style.sizeable = 1;
    params.style.moveable = 1;
    params.style.primary = 1;
    params.style.need_rtc = 1;
    params.state.active = 0;
    params.state.enabled = 1;
    params.state.selected = 0;
    params.state.visible = 1;
    params.context_id = AW_CONTEXT_ID_NEXT;
    params.buffer_id = AW_BUFFER_ID_NEXT;
    params.bitmap_id = AW_BITMAP_ID_NEXT;
    params.x = 20;
    params.y = 200;
    params.width = 204;
    params.height = 214;
    params.text = "WinClock";
    params.extra_data_size = 0;

    AwWindow* win = (*core->create_window)(&params);

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
} hour_points[12] = {
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

/*
    The table below was produced by this code (and then cleaned up a bit):

    #include <stdio.h>
    #include <math.h>

    int main()
    {
        for (int i = 0; i < 60; i++) {
            double angle = M_PI*i/6.0/5;
            int mx = (int)(cos(angle-M_PI/2) * 256);
            int my = (int)(sin(angle-M_PI/2) * 256);
            printf("{ %i, %i }, // [%02i] time 00:%02i, angle %f\n", mx, my, i, i, angle);
        }
        return 0;
    }
*/

const struct {
    int16_t x_mul;
    int16_t y_mul;
} second_points[60] = {
    { 0, -256 }, // [00] time 00:00, angle 0.000000
    { 26, -254 }, // [01] time 00:01, angle 0.104720
    { 53, -250 }, // [02] time 00:02, angle 0.209440
    { 79, -243 }, // [03] time 00:03, angle 0.314159
    { 104, -233 }, // [04] time 00:04, angle 0.418879
    { 128, -221 }, // [05] time 00:05, angle 0.523599
    { 150, -207 }, // [06] time 00:06, angle 0.628319
    { 171, -190 }, // [07] time 00:07, angle 0.733038
    { 190, -171 }, // [08] time 00:08, angle 0.837758
    { 207, -150 }, // [09] time 00:09, angle 0.942478
    { 221, -127 }, // [10] time 00:10, angle 1.047198
    { 233, -104 }, // [11] time 00:11, angle 1.151917
    { 243, -79 }, // [12] time 00:12, angle 1.256637
    { 250, -53 }, // [13] time 00:13, angle 1.361357
    { 254, -26 }, // [14] time 00:14, angle 1.466077
    { 256, 0 }, // [15] time 00:15, angle 1.570796
    { 254, 26 }, // [16] time 00:16, angle 1.675516
    { 250, 53 }, // [17] time 00:17, angle 1.780236
    { 243, 79 }, // [18] time 00:18, angle 1.884956
    { 233, 104 }, // [19] time 00:19, angle 1.989675
    { 221, 128 }, // [20] time 00:20, angle 2.094395
    { 207, 150 }, // [21] time 00:21, angle 2.199115
    { 190, 171 }, // [22] time 00:22, angle 2.303835
    { 171, 190 }, // [23] time 00:23, angle 2.408554
    { 150, 207 }, // [24] time 00:24, angle 2.513274
    { 127, 221 }, // [25] time 00:25, angle 2.617994
    { 104, 233 }, // [26] time 00:26, angle 2.722714
    { 79, 243 }, // [27] time 00:27, angle 2.827433
    { 53, 250 }, // [28] time 00:28, angle 2.932153
    { 26, 254 }, // [29] time 00:29, angle 3.036873
    { 0, 256 }, // [30] time 00:30, angle 3.141593
    { -26, 254 }, // [31] time 00:31, angle 3.246312
    { -53, 250 }, // [32] time 00:32, angle 3.351032
    { -79, 243 }, // [33] time 00:33, angle 3.455752
    { -104, 233 }, // [34] time 00:34, angle 3.560472
    { -128, 221 }, // [35] time 00:35, angle 3.665191
    { -150, 207 }, // [36] time 00:36, angle 3.769911
    { -171, 190 }, // [37] time 00:37, angle 3.874631
    { -190, 171 }, // [38] time 00:38, angle 3.979351
    { -207, 150 }, // [39] time 00:39, angle 4.084070
    { -221, 127 }, // [40] time 00:40, angle 4.188790
    { -233, 104 }, // [41] time 00:41, angle 4.293510
    { -243, 79 }, // [42] time 00:42, angle 4.398230
    { -250, 53 }, // [43] time 00:43, angle 4.502949
    { -254, 26 }, // [44] time 00:44, angle 4.607669
    { -256, 0 }, // [45] time 00:45, angle 4.712389
    { -254, -26 }, // [46] time 00:46, angle 4.817109
    { -250, -53 }, // [47] time 00:47, angle 4.921828
    { -243, -79 }, // [48] time 00:48, angle 5.026548
    { -233, -104 }, // [49] time 00:49, angle 5.131268
    { -221, -128 }, // [50] time 00:50, angle 5.235988
    { -207, -150 }, // [51] time 00:51, angle 5.340708
    { -190, -171 }, // [52] time 00:52, angle 5.445427
    { -171, -190 }, // [53] time 00:53, angle 5.550147
    { -150, -207 }, // [54] time 00:54, angle 5.654867
    { -127, -221 }, // [55] time 00:55, angle 5.759587
    { -104, -233 }, // [56] time 00:56, angle 5.864306
    { -79, -243 }, // [57] time 00:57, angle 5.969026
    { -53, -250 }, // [58] time 00:58, angle 6.073746
    { -26, -254 }, // [59] time 00:59, angle 6.178466
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

        uint8_t skip = 0;
        int16_t dot_radius = length / 80;
        radius = (length * 31) / 80;
        for (int16_t i = 0; i < 60; i++) {
            if (skip++ == 0) {
                continue;
            }
            int16_t x_pos = center_x + (radius * second_points[i].x_mul) / 256;
            int16_t y_pos = center_y + (radius * second_points[i].y_mul) / 256;
            vdp_move_to(x_pos, y_pos);
            vdp_plot(0x9D, x_pos + dot_radius, y_pos + dot_radius);
            if (skip == 5) {
                skip = 0;
            }
        }

        dot_radius = length / 60;
        for (int16_t i = 0; i < 12; i++) {
            int16_t x_pos = center_x + (radius * hour_points[i].x_mul) / 256;
            int16_t y_pos = center_y + (radius * hour_points[i].y_mul) / 256;
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
