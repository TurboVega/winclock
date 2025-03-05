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
    win->bg_color = 5;
    win->fg_color = 7;

	return 0;
}

int32_t winclock_handle_message(AwWindow* window, AwMsg* msg, bool* halt) {
    (void)window; // presently unused
    (void)msg; // presently unused
    (void)halt; // presently unused
	return 0; // default to core processing
}
