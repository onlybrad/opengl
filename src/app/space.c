#include "../core/visual/window.h"
#include "../core/visual/camera.h"
#include "../core/visual/scene3d.h"
#include "../core/opengl/shader.h"
#include "../core/opengl/texture.h"
#include "../core/object/object.h"
#include "../core/object/cube.h"
#include "../core/object/rectangle.h"
#include "../core/object/cylinder.h"
#include "../core/util/color.h"
#include "../core/util/util.h"
#include "../core/input/keys.h"
#include "../core/input/keyboard.h"
#include "../core/input/mouse.h"
#include "space.h"

#define ERR1(BOOL) if(!(BOOL)) return false
#define ERR2(BOOL) if(!(BOOL)) return 1
#define ERR3(INT)  if((INT) == -1) return false

static int background_id, cube_id, cube2_id, cylinder_id, light_id;
static struct OB_Object background, cube, cube2, cylinder, light;
static struct OB_Shader shader;
static struct OB_Camera camera;
static struct OB_Scene3D scene;
static struct OB_Texture space_texture, wood_texture;
static vec4 vec4_light_color;
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

static void input_callback(void) {   
    if (OB_Keyboard_is_pressed(OB_KEY_UP)) {
        OB_Camera_move_forward(&camera);
    } 
    
    if (OB_Keyboard_is_pressed(OB_KEY_DOWN)) {
        OB_Camera_move_backward(&camera);
    }

    if (OB_Keyboard_is_pressed(OB_KEY_RIGHT)) {
        OB_Camera_strafe_right(&camera);
    }

    if (OB_Keyboard_is_pressed(OB_KEY_LEFT)) {
        OB_Camera_strafe_left(&camera);
    }

    if(OB_Keyboard_is_pressed(OB_KEY_KP_8) && !OB_Keyboard_numlock()) {
        OB_Camera_move_up(&camera);
    }

    if(OB_Keyboard_is_pressed(OB_KEY_KP_2) && !OB_Keyboard_numlock()) {
        OB_Camera_move_down(&camera);
    }
}

static void mouse_cursor_callback(double x, double y) {
    OB_Camera_change_direction(&camera, (float)x, (float)y);
}

static void mouse_scroll_callback(double _, double yoffset) {
    (void)_;
    OB_Camera_zoom(&camera, (float)yoffset);
}

static bool shaders_init(void) {
    ERR1(OB_Shader_init(&shader, "./shaders/vertex.glsl", "./shaders/fragment.glsl"));

    OB_Shader_use(&shader);

    return true;
}

static void callbacks_init(void) {
    OB_Window_set_input_callback(input_callback);
    OB_Mouse_set_cursor_callback(mouse_cursor_callback);
    OB_Mouse_set_scroll_callback(mouse_scroll_callback);
}

static bool textures_init(void) {
    ERR1(OB_Texture_init(&space_texture, "spaceTexture", "./textures/space.jpg"));

    ERR1(OB_Texture_init(&wood_texture, "woodTexture", "./textures/wood.png"));

    return true;
}

static void camera_init(void) {
    const int width = OB_Window_get()->width;
    const int height = OB_Window_get()->height;

    OB_Camera_init(&camera, (float)width/2.0f, (float)height/2.0f);
    OB_Camera_set_speed(&camera, 0.05f);
    OB_Camera_set_position(&camera, camera_position);
    OB_Camera_set_fov(&camera, glm_rad(45.0f));
    OB_Camera_set_aspect(&camera, (float)width/(float)height);
    OB_Camera_set_near_z(&camera, 0.1f);
    OB_Camera_set_far_z(&camera, 100.0f);
}

//all opengl calls must be done in here
static void drawing_callback(void) {
    struct OB_Window *const window = OB_Window_get();
    const float time = OB_Window_time();
    const vec4 new_color = {
        MAX(0.2f, sinf(time * 2.0f)), 
        MAX(0.2f, sinf(time * 0.7f)), 
        MAX(0.2f, sinf(time * 1.3f)),
        1.0f
    };

    SceneObject3D *const scene_light = OB_Scene3D_object_get(window->scene, (unsigned)light_id);
    OB_Object_set_vec4_color(&light, (const float*)new_color);
    //Notify the scene that the light object has been updated, this will cause it to update the light object in the vertex buffer the next time OB_Scene3D_update_objects is called.
    OB_Scene3D_object_needs_update(window->scene, (unsigned)light_id);
    OB_Scene3D_update_objects(window->scene);

    OB_Shader_set_vec3(window->scene->shader, "light_position", scene_light->transform.translate);
    OB_Shader_set_vec4(window->scene->shader, "light_color", new_color);
    OB_Scene3D_draw_objects(window->scene);
}

//this runs on a seperate thread, opengl calls will fail if called from here. This is where CPU side logic must be implemented.
static void logic_callback(void) {
    struct OB_Window *const window = OB_Window_get();
    
    //Calculate the new rotation and position of the light source
    struct OB_Transform transform = OB_Scene3D_object_get(window->scene, (unsigned)light_id)->transform;
    transform.rotation_angle += glm_rad(1.0f);
    vec3 axis = {1.0f, 1.0f, 0.0f};
    glm_vec3_rotate(transform.translate, glm_rad(1.0f), axis);

    //update the Transform of the light object in the scene, this will automatically mark the light object as "needs_update" which will allow the drawing loop to update it in the vertex buffer once Scene3D_update_objects is called.
    OB_Scene3D_object_set_transform(window->scene, (unsigned)light_id, &transform);
}

static bool objects_init(void) {
    ERR1(OB_Rectangle_create_background(&background));
    OB_Object_set_texture(&background, &space_texture);
    background_id = OB_Scene3D_add_object(&scene, &background, NULL);

    ERR1(OB_Cube_create(&cube));
    OB_Object_set_texture(&cube, &wood_texture);
    ERR3(cube_id = OB_Scene3D_add_object(&scene, &cube, NULL));

    ERR1(OB_Cube_create(&cube2));
    OB_Object_set_texture(&cube2, &wood_texture);
    ERR3(cube2_id = OB_Scene3D_add_object(&scene, &cube2, &cube_transform));

    ERR1(OB_Cylinder_create(&cylinder, 0.5f, 0.5f));
    OB_Object_set_texture(&cylinder, &wood_texture);
    ERR3(cylinder_id = OB_Scene3D_add_object(&scene, &cylinder, &cylinder_transform));

    ERR1(OB_Cube_create_light(&light));
    OB_Object_set_rgba_color(&light, &light_color);
    ERR3(light_id = OB_Scene3D_add_object(&scene, &light, &light_transform));

    ERR1(OB_Shader_set_vec3(&shader, "light_position", light_transform.translate));
    ERR1(OB_Shader_set_vec4(&shader, "light_color", OB_Color_to_vec4(&light_color, vec4_light_color)));
    ERR1(OB_Shader_set_float(&shader, "ambiant_strength", 0.7f));

    return true;
}

int space_init(void) {
    OB_Keyboard_init();
    OB_Mouse_init();
    callbacks_init();
    camera_init();
    ERR2(shaders_init());
    ERR2(textures_init());
    ERR2(OB_Scene3D_init(&scene, &shader, &camera));
    ERR2(objects_init());

    OB_Window_set_scene3D(&scene);
    OB_Scene3D_start(&scene);
    OB_Window_logic_loop(logic_callback);
    OB_Window_drawing_loop(drawing_callback);
    OB_Scene3D_end(&scene);

    return 0;
}

void space_free(void) {
    OB_Texture_free(&space_texture);
    OB_Texture_free(&wood_texture);
    OB_Object_free(&cube);
    OB_Object_free(&cylinder);
    OB_Object_free(&light);
    OB_Object_free(&background);
    OB_Shader_free(&shader);
    OB_Scene3D_free(&scene);
    OB_Camera_free(&camera);
    OB_Window_free();
}
