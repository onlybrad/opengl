#include <string.h>
#include "Scene3D.h"
#include "OpenGL.h"
#include "Camera.h"
#include "../OpenGL/VertexLayout.h"
#include "../Thread/Thread.h"
#include "../Util/util.h"

extern VertexLayout OBJECT_VERTEX_LAYOUT;
extern const char TEXTURE_UNIFORMS[32][10];

static int get_texture_slot(Scene3D *const scene, Texture *const texture) {
    for(unsigned int i=0u; i<scene->objects_count; i++) {
        if(texture == scene->scene_objects[i].object->texture) {
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

void Scene3D_init(Scene3D *const scene, Shader *const shader, PerspectiveCamera *const perspective_camera) {
    VertexArrayObject_init(&scene->vao);
    scene->objects_count = 0u;
    scene->objects_vertices_count = 0u;
    scene->texture_slot = 1u;
    scene->to_update_count = 0u;
    scene->perspective_camera = perspective_camera;
    scene->shader = shader;
    scene->view_id = Shader_get_location(shader, "view");
    scene->projection_id = Shader_get_location(shader, "projection");
    perspective_camera->camera.id = Shader_get_location(shader, "camera_position");

    memset(scene->scene_objects, 0, MEMBER_SIZE(Scene3D,scene_objects));
    memset(scene->to_update, 0, MEMBER_SIZE(Scene3D,to_update));
}

void Scene3D_free(Scene3D *const scene) {
    VertexArrayObject_free(&scene->vao);
    VertexArrayBuffer_free(&scene->vab);
    memset(scene, 0, sizeof(Scene3D));
}

bool Scene3D_add_object(Scene3D *const scene, Object *const object, const Transform *const transform) {
    if(scene->objects_count >= ARRAY_MEMBER_LENGTH(Scene3D,scene_objects)) {
        return false;
    }

    scene->objects_vertices_count += object->vertices_count;

    scene->scene_objects[scene->objects_count++] = (Scene3DObject){
        .object = object, 
        .transform = transform == NULL 
            ? (Transform){.scale = {1.0f, 1.0f, 1.0f}} 
            : *transform,
        .needs_update = true
    };

    if(object->texture != NULL) {
        const int texture_slot = get_texture_slot(scene, object->texture);

        if(texture_slot == -1) {
            Object_set_texture_slot(object, scene->texture_slot);
            scene->texture_slot++;
        } else {
            Object_set_texture_slot(object, (unsigned int)texture_slot);
        }
    }

    return true;
}

void Scene3D_object_needs_update(Scene3D *const scene, const unsigned int object_index) {
    Scene3DObject *const scene_object = Scene3D_object_get(scene, object_index);

    Lock_lock(&scene->vab.lock);
    if(scene_object->needs_update) {
        scene->to_update[scene->to_update_count++] = object_index;
    } else {
        scene_object->needs_update = false;
    }
    Lock_unlock(&scene->vab.lock);
}

void Scene3D_object_set_transform(Scene3D *const scene, const unsigned int object_index, Transform *const transform) {
    Scene3DObject *const scene_object = Scene3D_object_get(scene, object_index);
    scene_object->transform = *transform;

    mat4 model;
    get_model(model, &scene_object->transform);
    Object_set_model(scene_object->object, (float*)model);
    Scene3D_object_needs_update(scene, object_index);
}

void Scene3D_set_background1(Scene3D *const scene, Object *const background) {
    scene->background = background;
    Object_set_texture_slot(background, 0u);
}

inline Scene3DObject *Scene3D_object_get(Scene3D *const scene, const unsigned int object_index) {
    return &scene->scene_objects[object_index];
}

void Scene3D_start1(Scene3D *const scene) {
    unsigned int buffer_size = 0u;
    unsigned int previous_offset = 0u;
    unsigned int previous_size = 0u;

    if(scene->background != NULL) {
        buffer_size += scene->background->vertices_count * (unsigned int)sizeof(*scene->background->vertices);
        previous_size += scene->background->vertices_count;
    }

    for(unsigned int i=0u; i < scene->objects_count; i++) {
        buffer_size += scene->scene_objects[i].object->vertices_count * (unsigned int)sizeof(*scene->scene_objects[i].object->vertices);
        scene->scene_objects[i].offset = previous_offset + previous_size;
        previous_offset = scene->scene_objects[i].offset;
        previous_size = scene->scene_objects[i].object->vertices_count;
    }

    VertexArrayBuffer_init(&scene->vab, NULL, buffer_size);

    if(scene->background != NULL) {
        VertexArrayBuffer_push(&scene->vab, scene->background->vertices, scene->background->vertices_count * sizeof(*scene->background->vertices));
        Texture_use(scene->background->texture, 0u);
        Shader_set_int(scene->shader, TEXTURE_UNIFORMS[0], 0u);
    }

    for(unsigned int i=0u; i < scene->objects_count; i++) {
        Scene3DObject *const scene_object = &scene->scene_objects[i];

        mat4 model;
        get_model(model, &scene_object->transform);

        Object_set_model(scene_object->object, (float*)model);
        VertexArrayBuffer_push(&scene->vab, scene_object->object->vertices, scene_object->object->vertices_count * sizeof(*scene_object->object->vertices));

        if(scene_object->object->texture != NULL) {
            Texture_use(scene_object->object->texture, Object_get_texture_slot(scene_object->object));
            Shader_set_int(scene->shader, TEXTURE_UNIFORMS[Object_get_texture_slot(scene_object->object)], (int)Object_get_texture_slot(scene_object->object));
        }
    }

    VertexArrayObject_add_buffer(&scene->vao, &scene->vab, NULL, &OBJECT_VERTEX_LAYOUT);
    Shader_set_mat4_l(scene->shader, scene->projection_id, PerspectiveCamera_get_projection(scene->perspective_camera));
}

void Scene3D_end1(Scene3D *const scene) {
    VertexArrayObject_unbind(&scene->vao);
    VertexArrayBuffer_free(&scene->vab);
    VertexArrayBuffer_unbind(&scene->vab);
}

void Scene3D_draw_objects(Scene3D *const scene) {
    OpenGL_clear();

    Lock_lock(&scene->perspective_camera->camera.lock);
    Shader_set_mat4_l(scene->shader, scene->projection_id, PerspectiveCamera_get_projection(scene->perspective_camera));
    Shader_set_mat4_l(scene->shader, scene->view_id, PerspectiveCamera_get_view(scene->perspective_camera));
    Shader_set_vec3_l(scene->shader, scene->perspective_camera->camera.id, scene->perspective_camera->camera.position);
    Lock_unlock(&scene->perspective_camera->camera.lock);

    int first = 0;
    if(scene->background != NULL) {
        OpenGL_draw_background(first, (int)scene->background->vertices_count);
        first += (int)scene->background->vertices_count;
    }

    OpenGL_draw(first, (int)scene->objects_vertices_count);
}

void Scene3D_update_objects(Scene3D *const scene) {
    Lock_lock(&scene->vab.lock);
    for(unsigned int i=0u; i<scene->to_update_count; i++) {
        Scene3DObject *const scene_object = &scene->scene_objects[scene->to_update[i]]; 
        VertexArrayBuffer_set(&scene->vab, scene_object->offset * sizeof(*scene_object->object->vertices), scene_object->object->vertices, scene_object->object->vertices_count * sizeof(*scene_object->object->vertices));
        scene_object->needs_update = true;
    }
    scene->to_update_count = 0u;
    Lock_unlock(&scene->vab.lock);
}