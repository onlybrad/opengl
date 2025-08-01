#ifndef OB_KEYBOARD_H
#define OB_KEYBOARD_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "../visual/window.h"
#include "Keys.h"

typedef void (*OB_Keyboard_Callback)(int key, int scancode, int action, int mods);

void OB_Keyboard_init(void);
void OB_Keyboard_set_callback(OB_Keyboard_Callback callback);
bool OB_Keyboard_is_pressed(int key);
bool OB_Keyboard_numlock(void);

#endif

