#include "core/Renderer/Window.h"
#include "core/Renderer/Camera.h"
#include "core/Renderer/Scene3D.h"
#include "core/OpenGL/Shader.h"
#include "core/OpenGL/Texture.h"
#include "core/Object/Object.h"
#include "core/Object/Cube.h"
#include "core/Object/Cylinder.h"
#include "core/Util/Color.h"
#include "app/callback.h"

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
        return 1;        
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
    Scene3D_set_background(&scene, &background);

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
    Shader_set_vec4(&shader, "light_color", COLOR_TO_VEC4(&light_color));
    Shader_set_float(&shader, "ambiant_strength", 0.7f);

    Window_set_scene3D(&window, &scene);

    Scene3D_start(&scene);
    Window_logic_loop(&window, logic_callback);
    Window_drawing_loop(&window, drawing_callback);
    Scene3D_end(&scene);

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