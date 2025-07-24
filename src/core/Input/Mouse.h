#ifndef OB_MOUSE_H
#define OB_MOUSE_H

#include "../Window/Window.h"

typedef void(*OB_MouseCallback)(struct OB_Window*, double, double);

void OB_Mouse_init(struct OB_Window *window);
void OB_Mouse_set_cursor_callback(OB_MouseCallback cursor_callback);
void OB_Mouse_set_scroll_callback(OB_MouseCallback scroll_callback);

#endif