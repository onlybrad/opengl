#include "core/Renderer/Window.h"
#include "core/Renderer/Camera.h"
#include "core/Renderer/Scene3D.h"
#include "core/Input/Keyboard.h"
#include "core/Input/Mouse.h"
#include "core/OpenGL/Shader.h"
#include "core/OpenGL/Texture.h"
#include "core/Object/Object.h"
#include "core/Object/Cube.h"
#include "core/Object/Cylinder.h"
#include "core/Util/Color.h"

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

int main(void) {
    const int width = 800;
    const int height = 600;

    Window window;
    Window_init(&window, width, height, "LearnOpenGL");
    Window_set_vsync(&window, true);
    Window_set_updates_per_second(&window, 60);
    Window_set_input_callback(&window, input_callback);

    Keyboard_init(&window);

    Mouse_init(&window);
    Mouse_set_cursor_callback(mouse_cursor_callback);
    Mouse_set_scroll_callback(mouse_scroll_callback);

    Shader shader;
    if(! Shader_init(&shader, "./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl")) {
        Window_free(&window);
        return -1;        
    }
    Shader_use(&shader);

    PerspectiveCamera camera;
    PerspectiveCamera_init(&camera, (float)width/2.0f, (float)height/2.0f);
    PerspectiveCamera_set_speed(&camera, 0.05f);
    PerspectiveCamera_set_position(&camera, (vec3){0.0f, 0.0f, 3.0f});
    PerspectiveCamera_set_fov(&camera, glm_rad(45.0f));
    PerspectiveCamera_set_aspect(&camera, (float)width/(float)height);
    PerspectiveCamera_set_near_z(&camera, 0.1f);
    PerspectiveCamera_set_far_z(&camera, 100.0f);
    
    Scene3D scene;
    Scene3D_init(&scene, &shader, &camera);
    
    Texture space_texture, wood_texture;
    Texture_init(&space_texture, "spaceTexture", "./textures/space.jpg");
    Texture_init(&wood_texture, "woodTexture", "./textures/wood.png");

    Object background, cube, cylinder, light;
    Cube_create_background(&background);
    Object_set_texture(&background, &space_texture);
    Scene3D_set_background1(&scene, &background);

    Cube_create(&cube);
    Object_set_texture(&cube, &wood_texture);
    Scene3D_add_object(&scene, &cube, NULL);

    Cylinder_create(&cylinder, 0.5f, 0.5f);
    Object_set_texture(&cylinder, &wood_texture);
    Scene3D_add_object(&scene, &cylinder, &(Transform){
        .scale = {1.0f, 1.0f, 1.0f},
        .translate = {2.0f, 0.0f, 0.0f}
    });

    Cube_create_light(&light);
    Transform light_transform = {
        .translate = {2.0f, 0.0f, 1.0f},
        .rotation_axis = {1.0f, 1.0f, 0.0f},
        .scale = {0.2f, 0.2f, 0.2f}, 
    };
    Color light_color = {255, 255, 255, 255};
    Object_set_rgba_color(&light, &light_color);
    Scene3D_add_object(&scene, &light, &light_transform);
    Shader_set_vec3(&shader, "light_position", light_transform.translate);
    Shader_set_vec4(&shader, "light_color", Color_to_vec4(&light_color, (vec4){0}));
    Shader_set_float(&shader, "ambiant_strength", 0.7f);

    Window_set_scene3D(&window, &scene);

    Scene3D_start1(&scene);
    Window_logic_loop(&window, logic_callback);
    Window_drawing_loop(&window, drawing_callback);
    Scene3D_end1(&scene);

    Texture_free(&space_texture);
    Texture_free(&wood_texture);
    Object_free(&cube);
    Object_free(&cylinder);
    Object_free(&light);
    Object_free(&background);
    Shader_free(&shader);
    Scene3D_free(&scene);
    Window_free(&window);
    
    return 0;
}