#include "core/Renderer/Window.h"
#include "core/Input/Keyboard.h"
#include "core/Input/Mouse.h"

static void input_callback(Window *const window) {
    PerspectiveCamera *const camera = window->scene->perspective_camera;
    
    if (Keyboard_is_pressed(GLFW_KEY_UP)) {
        PerspectiveCamera_move_front(camera);
    } 
    
    if (Keyboard_is_pressed(GLFW_KEY_DOWN)) {
        PerspectiveCamera_move_back(camera);
    }

    if (Keyboard_is_pressed(GLFW_KEY_RIGHT)) {
        PerspectiveCamera_move_right(camera);
    }

    if (Keyboard_is_pressed(GLFW_KEY_LEFT)) {
        PerspectiveCamera_move_left(camera);
    }

    if(Keyboard_is_pressed(GLFW_KEY_KP_8) && !Keyboard_numlock()) {
        PerspectiveCamera_move_straight_up(camera);
    }

    if(Keyboard_is_pressed(GLFW_KEY_KP_2) && !Keyboard_numlock()) {
        PerspectiveCamera_move_straight_down(camera);
    }
}

//all opengl calls must be done in here
static void drawing_callback(Window *const window) {
    Scene3D_update_objects1(window->scene);

    const unsigned int light_index = 2;

    Transform *const light_transform = &Scene3D_object1_get(window->scene, light_index)->transform;
    Shader_set_vec3(window->scene->shader, "light_position", light_transform->translate);

    Scene3D_draw_objects1(window->scene);
}

//this runs on a seperate thread, opengl calls will fail if called from here. This is where CPU side logic must be implemented.
static void logic_callback(Window *const window) {
    const unsigned int light_index = 2;
    
    Transform light_transform = Scene3D_object1_get(window->scene, light_index)->transform;
    light_transform.rotation_angle += glm_rad(1.0f);
    glm_vec3_rotate(light_transform.translate, glm_rad(1.0f), (vec3){1.0f, 1.0f, 0.0f});

    Scene3D_object1_set_transform(window->scene, light_index, &light_transform);
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
        glfwTerminate();
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

    Object1 background, cube1, cube2, light;
    Object1_create_background(&background);
    Object1_set_texture(&background, &space_texture);
    Scene3D_set_background1(&scene, &background);

    Object1_create_cube(&cube1);
    Object1_set_color(&cube1, (vec4){1.0f, 0.5f, 0.31f, 1.0f});
    Object1_set_texture(&cube1, &wood_texture);
    Scene3D_add_object1(&scene, &cube1, NULL);

    Object1_create_cube(&cube2);
    Object1_set_color(&cube2, (vec4){0.5f, 0.31f, 1.0f, 1.0f});
    Object1_set_texture(&cube2, &wood_texture);
    Scene3D_add_object1(&scene, &cube2, &(Transform){
        .scale = {1.0f, 1.0f, 1.0f},
        .translate = {2.0f, 0.0f, 0.0f}
    });

    Object1_create_light_cube(&light);
    Transform light_transform = {
        .translate = {2.0f, 0.0f, 1.0f},
        .rotation_axis = {1.0f, 1.0f, 0.0f},
        .scale = {0.2f, 0.2f, 0.2f}, 
    };
    vec4 light_color = {1.0f, 1.0f, 1.0f, 1.0f};
    Object1_set_color(&light, light_color);
    Scene3D_add_object1(&scene, &light, &light_transform);
    Shader_set_vec3(&shader, "light_position", light_transform.translate);
    Shader_set_vec4(&shader, "light_color", light_color);

    Window_set_scene3D(&window, &scene);

    Scene3D_start1(&scene);
    Window_logic_loop(&window, logic_callback);
    Window_drawing_loop(&window, drawing_callback);
    Scene3D_end1(&scene);

    Texture_free(&space_texture);
    Shader_free(&shader);
    Scene3D_free(&scene);
    Object1_free(&cube1);
    Object1_free(&cube2);
    Object1_free(&light);
    Object1_free(&background);
    Window_free(&window);
    
    return 0;
}