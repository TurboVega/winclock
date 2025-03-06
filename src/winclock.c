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

extern AwAppHeader _agwin_header;
const AwFcnTable* core;

// The "my..." code and data is used for a simple test, and will
// be removed once there is a better example of a loaded app.

int32_t winclock_handle_message(AwWindow* window, AwMsg* msg, bool* halt);

AwClass winclock_class = { "winclock", NULL, winclock_handle_message };

AwApplication winclock_app = { "winclock", 0, 0, &winclock_class, NULL, 1 };

int main( void )
{
    _agwin_header.app = &winclock_app;
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

    AwWindowState state;
    state.active = 0;
    state.enabled = 1;
    state.selected = 0;
    state.visible = 1;

    AwWindow* win = (*core->create_window)(&winclock_app, NULL, &winclock_class,
        style, state, 320, 240, 100, 100, "WinClock", 0);
    if (win) {
        win->bg_color = 5;
        win->fg_color = 7;
    }

	return 0;
}

int32_t on_paint_window(AwWindow* window, AwMsg* msg, bool* halt) {
    *halt = true; // no more handling after this
    if (!window->state.visible) {
        return 0;
    }

    (*core->paint_window)(msg); // paint borders and title bar, if needed

return 0;
    AwDoMsgPaintWindow* paint_msg = &msg->do_paint_window;

    AwPaintFlags* paint_flags = &paint_msg->flags;
    if (paint_flags->client || paint_flags->window) {
        (*core->set_client_viewport)(window);
        vdp_set_graphics_colour(0, window->bg_color | 0x80);
        vdp_set_graphics_colour(0, window->fg_color);
        int16_t center_x = (window->client_rect.left + window->client_rect.right) / 2;
        int16_t center_y = (window->client_rect.top + window->client_rect.bottom) / 2;
        vdp_move_to(center_x - 10, center_y - 10);
        vdp_plot(0x95, center_x + 10 - 1, center_y + 10 - 1);
    }

    return 0;
}

int32_t winclock_handle_message(AwWindow* window, AwMsg* msg, bool* halt) {
    switch (msg->do_common.msg_type) {
        case Aw_Do_PaintWindow: {
            //on_paint_window(window, msg, halt);
            break;
        }

        default: {
            break;
        }
    }
	return 0; // default to core processing
}
