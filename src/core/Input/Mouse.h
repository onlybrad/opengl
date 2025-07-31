#ifndef OB_MOUSE_H
#define OB_MOUSE_H

#include "../visual/window.h"

typedef void(*OB_Mouse_Callback)(struct OB_Window *, double x, double y);

void OB_Mouse_init(void);
void OB_Mouse_set_cursor_callback(OB_Mouse_Callback cursor_callback);
void OB_Mouse_set_scroll_callback(OB_Mouse_Callback scroll_callback);

#endif