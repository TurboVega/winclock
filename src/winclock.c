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

#include "../../agwin/src/agwindefs.h"

extern AwApplication* _agwin_ptr_app;

// The "my..." code and data is used for a simple test, and will
// be removed once there is a better example of a loaded app.

int32_t winclock_handle_message(AwWindow* window, AwMsg* msg, bool* halt);

AwClass winclock_class = { "winclock", NULL, winclock_handle_message };

AwApplication winclock_app = { "winclock", 0, 0, &winclock_class, NULL, 1 };

int main( void )
{
    _agwin_ptr_app = &winclock_app;
	return 0;
}

int32_t winclock_handle_message(AwWindow* window, AwMsg* msg, bool* halt) {
    (void)window; // presently unused
    (void)msg; // presently unused
    (void)halt; // presently unused
	return 0; // default to core processing
}
