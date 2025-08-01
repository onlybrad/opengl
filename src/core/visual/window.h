#ifndef OB_WINDOW_GLFW_H
#define OB_WINDOW_GLFW_H

#include <GLFW/glfw3.h>
#include "../visual/scene3d.h"

struct OB_Window;

typedef void (*OB_Window_Callback)(void);

struct OB_Window {
    bool should_close;
    int width;
    int height;
    unsigned updates_per_second;
    float delta_time;
    double current_time;
    unsigned long long frame;
    GLFWwindow *glfw_window;
    struct OB_Scene3D *scene;
    OB_Window_Callback input_callback;
};

bool OB_Window_init(int width, int height, const char* title);
void OB_Window_free(void);
struct OB_Window *OB_Window_get(void);
void OB_Window_set_vsync(bool on);
void OB_Window_set_scene3D(struct OB_Scene3D *scene);
void OB_Window_set_updates_per_second(unsigned updates_per_seconds);
void OB_Window_drawing_loop(OB_Window_Callback drawing_callback);
void OB_Window_logic_loop(OB_Window_Callback logic_callback);
void OB_Window_set_input_callback(OB_Window_Callback input_callback);
float OB_Window_time(void);

#endif