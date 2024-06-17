#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "../Renderer/Window.h"
#include "_Keys.h"

typedef void (*Keyboard_Callback)(Window *const, int key, int scancode, int action, int mods);

void Keyboard_init(Window window[static 1]);
void Keyboard_set_callback(Keyboard_Callback callback);
bool Keyboard_is_pressed(const int key);
bool Keyboard_numlock(void);

#endif

