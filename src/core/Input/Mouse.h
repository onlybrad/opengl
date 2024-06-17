#ifndef _MOUSE_H
#define _MOUSE_H

#include "../Renderer/Window.h"

typedef void(*MouseCursorCallback)(Window *const, double, double);
typedef void(*MouseScrollCallback)(Window *const, double, double);

void Mouse_init(Window window[static 1]);
void Mouse_set_cursor_callback(MouseScrollCallback cursor_callback);
void Mouse_set_scroll_callback(MouseScrollCallback scroll_callback);

#endif