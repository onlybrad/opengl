#ifndef OB_KEYBOARD_H
#define OB_KEYBOARD_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "../Window/Window.h"
#include "Keys.h"

typedef void (*OB_Keyboard_Callback)(struct OB_Window *, int key, int scancode, int action, int mods);

void OB_Keyboard_init(struct OB_Window *window);
void OB_Keyboard_set_callback(OB_Keyboard_Callback callback);
bool OB_Keyboard_is_pressed(int key);
bool OB_Keyboard_numlock(void);

#endif

