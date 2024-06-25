#include <string.h>
#include <stdlib.h>
#include "Scene3D.h"
#include "OpenGL.h"
#include "Camera.h"
#include "../OpenGL/VertexLayout.h"
#include "../Thread/Thread.h"
#include "../Util/util.h"

#define T SceneObject3D
#define VECTOR_IMPLEMENTATION
#include "../Template/Vector.h"

#define T uint
#define VECTOR_IMPLEMENTATION
#include "../Template/Vector.h"

extern VertexLayout OBJECT_VERTEX_LAYOUT;
extern const char TEXTURE_UNIFORMS[32][10];

static int find_texture_slot(Scene3D scene[static 1], Texture *const texture) {
    for(unsigned int i=0u; i<scene->scene_objects.length; i++) {
        if(texture == scene->scene_objects.buffer[i].object->texture) {
            return (int)i+1;
        }
    }
    return -1;
}

static void get_model(mat4 model, Transform *const transform) {
    glm_mat4_identity(model);
    glm_translate(model, transform->translate);
    glm_rotate(model, transform->rotation_angle, transform->rotation_axis);
    glm_scale(model, transform->scale);
}

void Scene3D_init(Scene3D scene[static 1], Shader shader[static 1], PerspectiveCamera perspective_camera[static 1]) {
    VertexArrayObject_init(&scene->vao);
    scene->vertices_count = 0u;
    scene->texture_slot = 1u;
    scene->perspective_camera = perspective_camera;
    scene->shader = shader;
    Vector_SceneObject3D_init(&scene->scene_objects, 128);
    Vector_uint_init(&scene->to_update, 128u); 
    scene->view_id = Shader_get_location(shader, "view");
    scene->projection_id = Shader_get_location(shader, "projection");
    perspective_camera->camera.id = Shader_get_location(shader, "camera_position");
}

void Scene3D_free(Scene3D scene[static 1]) {
    VertexArrayObject_free(&scene->vao);
    VertexArrayBuffer_free(&scene->vab);
    Vector_SceneObject3D_free(&scene->scene_objects);
    Vector_uint_free(&scene->to_update);
}

bool Scene3D_add_object(Scene3D scene[static 1], Object object[static 1], const Transform *const transform) {
    scene->vertices_count += object->vertices_count;

    Vector_SceneObject3D_push(&scene->scene_objects, (SceneObject3D){
        .object = object, 
        .transform = transform == NULL 
            ? (Transform){.scale = {1.0f, 1.0f, 1.0f}} 
            : *transform,
        .needs_update = true
    });

    if(object->texture != NULL) {
        const int texture_slot = find_texture_slot(scene, object->texture);

        if(texture_slot == -1) {
            Object_set_texture_slot(object, scene->texture_slot);
            scene->texture_slot++;
        } else {
            Object_set_texture_slot(object, (unsigned int)texture_slot);
        }
    }

    return true;
}

void Scene3D_object_needs_update(Scene3D scene[static 1], const unsigned int object_index) {
    SceneObject3D *const scene_object = Scene3D_object_get(scene, object_index);

    SYNCHRONIZE(&scene->vab.lock, 
    if(scene_object->needs_update) {
        Vector_uint_push(&scene->to_update, object_index);
    } else {
        scene_object->needs_update = false;
    })
}

void Scene3D_object_set_transform(Scene3D scene[static 1], const unsigned int object_index, Transform transform[static 1]) {
    SceneObject3D *const scene_object = Scene3D_object_get(scene, object_index);
    scene_object->transform = *transform;

    mat4 model;
    get_model(model, &scene_object->transform);
    Object_set_model(scene_object->object, (float*)model);
    Scene3D_object_needs_update(scene, object_index);
}

void Scene3D_set_background(Scene3D scene[static 1], Object background[static 1]) {
    scene->background = background;
    Object_set_texture_slot(background, 0u);
}

inline SceneObject3D *Scene3D_object_get(Scene3D scene[static 1], const unsigned int object_index) {
    return &scene->scene_objects.buffer[object_index];
}

void Scene3D_start(Scene3D scene[static 1]) {
    unsigned int vab_size = 0u;
    unsigned int previous_offset = 0u;
    unsigned int previous_size = 0u;

    if(scene->background != NULL) {
        vab_size += scene->background->vertices_count * (unsigned int)sizeof(*scene->background->vertices);
        previous_size += scene->background->vertices_count;
    }
  
    for(unsigned int i=0u; i < (unsigned int)scene->scene_objects.length; i++) {
        Object *const object = scene->scene_objects.buffer[i].object;

        vab_size += object->vertices_count * (unsigned int)sizeof(*object->vertices);
        scene->scene_objects.buffer[i].offset = previous_offset + previous_size;
        previous_offset = scene->scene_objects.buffer[i].offset;
        previous_size = object->vertices_count;
    }

    VertexArrayBuffer_init(&scene->vab, NULL, vab_size);

    if(scene->background != NULL) {
        VertexArrayBuffer_push(&scene->vab, scene->background->vertices, scene->background->vertices_count * sizeof(*scene->background->vertices));
        Texture_use(scene->background->texture, 0u);
        Shader_set_int(scene->shader, TEXTURE_UNIFORMS[0], 0);
    }

    for(unsigned int i=0u; i < (unsigned int)scene->scene_objects.length; i++) {
        Object *const object = scene->scene_objects.buffer[i].object;

        mat4 model;
        get_model(model, &scene->scene_objects.buffer[i].transform);

        Object_set_model(object, (float*)model);
        VertexArrayBuffer_push(&scene->vab, object->vertices, object->vertices_count * sizeof(*object->vertices));

        if(object->texture != NULL) {
            Texture_use(object->texture, Object_get_texture_slot(object));
            Shader_set_int(scene->shader, TEXTURE_UNIFORMS[Object_get_texture_slot(object)], (int)Object_get_texture_slot(object));
        }
    }

    VertexArrayObject_add_buffer(&scene->vao, &scene->vab, NULL, &OBJECT_VERTEX_LAYOUT);
    Shader_set_mat4_l(scene->shader, scene->projection_id, PerspectiveCamera_get_projection(scene->perspective_camera));
}

void Scene3D_end(Scene3D scene[static 1]) {
    VertexArrayObject_unbind(&scene->vao);
    VertexArrayBuffer_free(&scene->vab);
    VertexArrayBuffer_unbind(&scene->vab);
}

void Scene3D_draw_objects(Scene3D scene[static 1]) {
    OpenGL_clear();

    SYNCHRONIZE(&scene->perspective_camera->camera.lock,
    Shader_set_mat4_l(scene->shader, scene->projection_id, PerspectiveCamera_get_projection(scene->perspective_camera));
    Shader_set_mat4_l(scene->shader, scene->view_id, PerspectiveCamera_get_view(scene->perspective_camera));
    Shader_set_vec3_l(scene->shader, scene->perspective_camera->camera.id, scene->perspective_camera->camera.position);)

    int first = 0;
    if(scene->background != NULL) {
        OpenGL_draw_background(first, (int)scene->background->vertices_count);
        first += (int)scene->background->vertices_count;
    }

    OpenGL_draw(first, (int)scene->vertices_count);
}

void Scene3D_update_objects(Scene3D scene[static 1]) {
    SYNCHRONIZE(&scene->vab.lock,
    for(unsigned int i=0u; i<(unsigned int)scene->to_update.length; i++) {
        SceneObject3D *const scene_object = &scene->scene_objects.buffer[scene->to_update.buffer[i]]; 
        VertexArrayBuffer_set(&scene->vab, scene_object->offset * sizeof(*scene_object->object->vertices), scene_object->object->vertices, scene_object->object->vertices_count * sizeof(*scene_object->object->vertices));
        scene_object->needs_update = true;
    }
    scene->to_update.length = 0;)
}