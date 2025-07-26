#include "../core/Window/Window.h"
#include "../core/Window/Camera.h"
#include "../core/Window/Scene3D.h"
#include "../core/OpenGL/Shader.h"
#include "../core/OpenGL/Texture.h"
#include "../core/Object/Object.h"
#include "../core/Object/Cube.h"
#include "../core/Object/Cylinder.h"
#include "../core/Util/Color.h"
#include "../core/Util/util.h"
#include "callback.h"

#define CHECK_ERROR(BOOL) do { if(!BOOL) {code = 1; goto cleanup;} } while(0) 

static struct OB_Shader shader = OB_ZERO;
static struct OB_PerspectiveCamera camera = OB_ZERO;
static struct OB_Scene3D scene = OB_ZERO;
static struct OB_Texture 
    space_texture = OB_ZERO, 
    wood_texture = OB_ZERO;
static struct OB_Object 
    background = OB_ZERO,
    cube = OB_ZERO,
    cube2 = OB_ZERO,
    cylinder = OB_ZERO,
    light = OB_ZERO;
static struct OB_Window window = OB_ZERO;
static vec4 vec4_light_color = OB_ZERO;

static const vec4 camera_position = {0.0f, 0.0f, 3.0f};
static const struct OB_Color light_color = {255, 255, 255, 255};
static const struct OB_Transform 
cube_transform = {
    0.0f, 
    {0.0f, 0.0f, 0.0f},
    {0.0f, 2.0f, 0.0f},
    {1.0f, 1.0f, 1.0f}
}, cylinder_transform = {
    0.0f,
    {0.0f, 0.0f, 0.0f},
    {2.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f}
}, light_transform = {
    0.0,
    {1.0f, 1.0f, 0.0f},
    {2.0f, 0.0f, 1.0f},
    {0.2f, 0.2f, 0.2f}, 
};

int main(void) {
    int code = 0;
    const int width = 800;
    const int height = 600;

    //Window
    OB_Window_init(&window, width, height, "LearnOpenGL");
    OB_Window_set_vsync(&window, true);
    OB_Window_set_updates_per_second(&window, 60);
    OB_Window_set_input_callback(&window, input_callback);

    //Keyboard
    OB_Keyboard_init(&window);

    //Mouse
    OB_Mouse_init(&window);
    OB_Mouse_set_cursor_callback(mouse_cursor_callback);
    OB_Mouse_set_scroll_callback(mouse_scroll_callback);

    //Shader
    CHECK_ERROR(OB_Shader_init(&shader, "./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl"));
    OB_Shader_use(&shader);

    //Camera
    OB_PerspectiveCamera_init(&camera, (float)width/2.0f, (float)height/2.0f);
    OB_PerspectiveCamera_set_speed(&camera, 0.05f);
    OB_PerspectiveCamera_set_position(&camera, camera_position);
    OB_PerspectiveCamera_set_fov(&camera, glm_rad(45.0f));
    OB_PerspectiveCamera_set_aspect(&camera, (float)width/(float)height);
    OB_PerspectiveCamera_set_near_z(&camera, 0.1f);
    OB_PerspectiveCamera_set_far_z(&camera, 100.0f);
    
    //Texture
    CHECK_ERROR(OB_Texture_init(&space_texture, "spaceTexture", "./textures/space.jpg"));
    CHECK_ERROR(OB_Texture_init(&wood_texture, "woodTexture", "./textures/wood.png"));

    //Scene
    CHECK_ERROR(OB_Scene3D_init(&scene, &shader, &camera));
    
    //Background
    CHECK_ERROR(OB_Cube_create_background(&background));
    OB_Object_set_texture(&background, &space_texture);
    OB_Scene3D_set_background(&scene, &background);

    //Cube 1
    CHECK_ERROR(OB_Cube_create(&cube));
    OB_Object_set_texture(&cube, &wood_texture);
    OB_Scene3D_add_object(&scene, &cube, NULL);

    //Cube 2
    CHECK_ERROR(OB_Cube_create(&cube2));
    OB_Object_set_texture(&cube2, &wood_texture);
    OB_Scene3D_add_object(&scene, &cube2, &cube_transform);

    //Cylinder
    CHECK_ERROR(OB_Cylinder_create(&cylinder, 0.5f, 0.5f));
    OB_Object_set_texture(&cylinder, &wood_texture);
    OB_Scene3D_add_object(&scene, &cylinder, &cylinder_transform);

    //Light
    CHECK_ERROR(OB_Cube_create_light(&light));
    OB_Object_set_rgba_color(&light, &light_color);
    CHECK_ERROR(OB_Scene3D_add_object(&scene, &light, &light_transform));
    CHECK_ERROR(OB_Shader_set_vec3(&shader, "light_position", light_transform.translate));
    CHECK_ERROR(OB_Shader_set_vec4(&shader, "light_color", OB_Color_to_vec4(&light_color, vec4_light_color)));
    CHECK_ERROR(OB_Shader_set_float(&shader, "ambiant_strength", 0.7f));

    //Add scene to windows then start the scene
    OB_Window_set_scene3D(&window, &scene);
    OB_Scene3D_start(&scene);
    OB_Window_logic_loop(&window, logic_callback);
    OB_Window_drawing_loop(&window, drawing_callback);
    OB_Scene3D_end(&scene);

cleanup:
    OB_Texture_free(&space_texture);
    OB_Texture_free(&wood_texture);
    OB_Object_free(&cube);
    OB_Object_free(&cylinder);
    OB_Object_free(&light);
    OB_Object_free(&background);
    OB_Shader_free(&shader);
    OB_Scene3D_free(&scene);
    OB_PerspectiveCamera_free(&camera);
    OB_Window_free(&window);
    
    return code;
}