#ifndef _WINDOW_GLFW_H
#define _WINDOW_GLFW_H

#include <GLFW/glfw3.h>
#include "../Renderer/Scene3D.h"

typedef struct Window Window;
typedef void(*WindowCallback)(Window *const);

typedef struct WindowThreadArgs {
    Window *window;
    WindowCallback callback;
} WindowThreadArgs;

struct Window {
    bool shoudClose;
    int width;
    int height;
    unsigned int updates_per_second;
    float delta_time;
    double current_time;
    long long unsigned int frame;
    GLFWwindow *glfw_window;
    Scene3D *scene;
    WindowCallback input_callback;
    WindowThreadArgs logic_thread_args;
};

void Window_init(Window window[static 1], const int width, const int height, const char* const title);
void Window_free(Window window[static 1]);
void Window_set_vsync(Window window[static 1], const bool on);
void Window_set_scene3D(Window window[static 1], Scene3D scene[static 1]);
void Window_set_updates_per_second(Window window[static 1], const unsigned int updates_per_seconds);
void Window_drawing_loop(Window window[static 1], void(*drawing_function)(Window *const));
void Window_logic_loop(Window window[static 1], void(*logic_function)(Window *const));
void Window_set_input_callback(Window window[static 1], WindowCallback input_callback);
float Window_time(void);

#endif