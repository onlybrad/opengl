#include <cglm/cglm.h>
#include "../core/Object/Object.h"
#include "../core/OpenGL/Shader.h"
#include "../core/Renderer/Window.h"
#include "../core/Renderer/Camera.h"
#include "../core/Renderer/Scene3D.h"
#include "../core/Input/Keyboard.h"
#include "../core/Input/Mouse.h"
#include "../core/Util/util.h"

static void input_callback(Window *const window) {
    PerspectiveCamera *const camera = window->scene->perspective_camera;
    
    if (Keyboard_is_pressed(KEY_UP)) {
        PerspectiveCamera_move_front(camera);
    } 
    
    if (Keyboard_is_pressed(KEY_DOWN)) {
        PerspectiveCamera_move_back(camera);
    }

    if (Keyboard_is_pressed(KEY_RIGHT)) {
        PerspectiveCamera_move_right(camera);
    }

    if (Keyboard_is_pressed(KEY_LEFT)) {
        PerspectiveCamera_move_left(camera);
    }

    if(Keyboard_is_pressed(KEY_KP_8) && !Keyboard_numlock()) {
        PerspectiveCamera_move_straight_up(camera);
    }

    if(Keyboard_is_pressed(KEY_KP_2) && !Keyboard_numlock()) {
        PerspectiveCamera_move_straight_down(camera);
    }
}

//all opengl calls must be done in here
static void drawing_callback(Window *const window) {
    const float time = Window_time();
    const vec4 new_color = {
        MAX(0.2f, sinf(time * 2.0f)), 
        MAX(0.2f, sinf(time * 0.7f)), 
        MAX(0.2f, sinf(time * 1.3f)),
        1.0f
    };

    //The index of the light object in the current scene
    const unsigned int light_index = 2;
    Scene3DObject *const scene_light = Scene3D_object_get(window->scene, light_index);
    Object_set_vec4_color(scene_light->object, (const float*)new_color);
    //Notify the scene that the light object has been updated, this will cause it to update the light object in the vertex buffer the next time Scene3D_update_objects is called.
    Scene3D_object_needs_update(window->scene, light_index);
    Scene3D_update_objects(window->scene);

    Shader_set_vec3(window->scene->shader, "light_position", scene_light->transform.translate);
    Shader_set_vec4(window->scene->shader, "light_color", new_color);
    Scene3D_draw_objects(window->scene);
}

//this runs on a seperate thread, opengl calls will fail if called from here. This is where CPU side logic must be implemented.
static void logic_callback(Window *const window) {
    //The index of the light object in the current scene
    const unsigned int light_index = 2;
    
    //Calculate the new rotation and position of the light source
    Transform light_transform = Scene3D_object_get(window->scene, light_index)->transform;
    light_transform.rotation_angle += glm_rad(1.0f);
    glm_vec3_rotate(light_transform.translate, glm_rad(1.0f), (vec3){1.0f, 1.0f, 0.0f});

    //update the Transform of the light object in the scene, this will automatically mark the light object as "needs_update" which will allow the drawing loop to update it in the vertex buffer once Scene3D_update_objects is called.
    Scene3D_object_set_transform(window->scene, light_index, &light_transform);
}

static void mouse_cursor_callback(Window *const window, double x, double y) {
    PerspectiveCamera_change_direction(window->scene->perspective_camera, (float)x, (float)y);
}

static void mouse_scroll_callback(Window *const window, double _, double yoffset) {
    (void)_;
    PerspectiveCamera_zoom(window->scene->perspective_camera, (float)yoffset);
}
