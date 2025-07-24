#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "Scene3D.h"
#include "Camera.h"
#include "../OpenGL/OpenGL.h"
#include "../OpenGL/VertexLayout.h"
#include "../Thread/Thread.h"
#include "../Util/util.h"

#define VECTOR_T SceneObject3D
#define VECTOR_IMPLEMENTATION
#include "../Template/Vector.h"

#define VECTOR_T uint
#define VECTOR_IMPLEMENTATION
#include "../Template/Vector.h"

extern struct OB_VertexLayout OB_OBJECT_VERTEX_LAYOUT;
extern const char OB_TEXTURE_UNIFORMS[32][10];

static int OB_find_texture_slot(Scene3D *scene, struct OB_Texture *texture) {
    assert(scene != NULL);
    assert(texture != NULL);

    for(unsigned i = 0u; i < scene->scene_objects.length; i++) {
        if(texture == scene->scene_objects.buffer[i].object->texture) {
            return (int)i+1;
        }
    }
    return -1;
}

static void OB_get_model(mat4 model, struct OB_Transform *transform) {
    assert(model != NULL);
    assert(transform != NULL);

    glm_mat4_identity(model);
    glm_translate(model, transform->translate);
    glm_rotate(model, transform->rotation_angle, transform->rotation_axis);
    glm_scale(model, transform->scale);
}

bool OB_Scene3D_init(struct OB_Scene3D *scene, struct OB_Shader *shader, struct OB_PerspectiveCamera *perspective_camera) {
    assert(scene != NULL);
    assert(shader != NULL);
    assert(perspective_camera != NULL);

    OB_VertexArrayObject_init(&scene->vao);
    scene->vertices_count = 0u;
    scene->texture_slot = 1u;
    scene->perspective_camera = perspective_camera;
    scene->shader = shader;

    if(!Vector_SceneObject3D_init(&scene->scene_objects, 128)) {
        return false;
    }

    if(!Vector_uint_init(&scene->to_update, 128)) {
        Vector_SceneObject3D_free(&scene->scene_objects);
        return false;
    } 

    scene->view_id = OB_Shader_get_location(shader, "view");
    scene->projection_id = OB_Shader_get_location(shader, "projection");
    perspective_camera->camera.id = OB_Shader_get_location(shader, "camera_position");

    return true;
}

void OB_Scene3D_free(struct OB_Scene3D *scene) {
    assert(scene != NULL);

    OB_VertexArrayObject_free(&scene->vao);
    OB_VertexArrayBuffer_free(&scene->vab);
    Vector_SceneObject3D_free(&scene->scene_objects);
    Vector_uint_free(&scene->to_update);

    memset(scene, 0, sizeof(*scene));
}

bool OB_Scene3D_add_object(struct OB_Scene3D *scene, struct OB_Object *object, const struct OB_Transform *transform) {
    assert(scene != NULL);
    assert(object != NULL);
    
    scene->vertices_count += object->vertices_count;

    struct OB_Transform t;
    if(transform == NULL) {
        memset(&t, 0, sizeof(t));
        t.scale[0] = 1.0;
        t.scale[1] = 1.0;
        t.scale[2] = 1.0;
    } else {
        t = *transform;
    }

    const struct OB_SceneObject3D object3D = {object, t, 0u, true};
    if(!Vector_SceneObject3D_push(&scene->scene_objects, object3D)) {
        return false;
    }

    if(object->texture != NULL) {
        const int texture_slot = OB_find_texture_slot(scene, object->texture);

        if(texture_slot == -1) {
            OB_Object_set_texture_slot(object, scene->texture_slot);
            scene->texture_slot++;
        } else {
            OB_Object_set_texture_slot(object, (unsigned)texture_slot);
        }
    }

    return true;
}

void OB_Scene3D_object_needs_update(struct OB_Scene3D *scene, unsigned object_index) {
    assert(scene != NULL);
    
    struct OB_SceneObject3D *const scene_object = OB_Scene3D_object_get(scene, object_index);

    OB_Lock_lock(&scene->vab.lock); 
    if(scene_object->needs_update) {
        Vector_uint_push(&scene->to_update, object_index);
    } else {
        scene_object->needs_update = false;
    }
    OB_Lock_unlock(&scene->vab.lock);
}

void OB_Scene3D_object_set_transform(struct OB_Scene3D *scene, unsigned object_index, struct OB_Transform *transform) {
    assert(scene != NULL);
    assert(transform != NULL);

    struct OB_SceneObject3D *const scene_object = OB_Scene3D_object_get(scene, object_index);
    scene_object->transform = *transform;

    mat4 model;
    OB_get_model(model, &scene_object->transform);
    OB_Object_set_model(scene_object->object, (float*)model);
    OB_Scene3D_object_needs_update(scene, object_index);
}

void OB_Scene3D_set_background(struct OB_Scene3D *scene, struct OB_Object *background) {
    assert(scene != NULL);
    assert(background != NULL);

    scene->background = background;
    OB_Object_set_texture_slot(background, 0u);
}

inline struct OB_SceneObject3D *OB_Scene3D_object_get(struct OB_Scene3D *scene, unsigned object_index) {
    assert(scene != NULL);

    return &scene->scene_objects.buffer[object_index];
}

void OB_Scene3D_start(struct OB_Scene3D *scene) {
    assert(scene != NULL);

    unsigned vab_size = 0u;
    unsigned previous_offset = 0u;
    unsigned previous_size = 0u;

    if(scene->background != NULL) {
        vab_size += scene->background->vertices_count * (unsigned)sizeof(*scene->background->vertices);
        previous_size += scene->background->vertices_count;
    }
  
    for(unsigned i = 0u; i < (unsigned)scene->scene_objects.length; i++) {
        struct OB_Object *const object = scene->scene_objects.buffer[i].object;

        vab_size += object->vertices_count * (unsigned)sizeof(*object->vertices);
        scene->scene_objects.buffer[i].offset = previous_offset + previous_size;
        previous_offset = scene->scene_objects.buffer[i].offset;
        previous_size = object->vertices_count;
    }

    OB_VertexArrayBuffer_init(&scene->vab, NULL, vab_size);

    if(scene->background != NULL) {
        OB_VertexArrayBuffer_push(&scene->vab, scene->background->vertices, scene->background->vertices_count * sizeof(*scene->background->vertices));
        OB_Texture_use(scene->background->texture, 0u);
        OB_Shader_set_int(scene->shader, OB_TEXTURE_UNIFORMS[0], 0);
    }

    for(unsigned i = 0u; i < (unsigned)scene->scene_objects.length; i++) {
       struct OB_Object *const object = scene->scene_objects.buffer[i].object;

        mat4 model;
        OB_get_model(model, &scene->scene_objects.buffer[i].transform);

        OB_Object_set_model(object, (float*)model);
        OB_VertexArrayBuffer_push(&scene->vab, object->vertices, object->vertices_count * sizeof(*object->vertices));

        if(object->texture != NULL) {
            OB_Texture_use(object->texture, OB_Object_get_texture_slot(object));
            OB_Shader_set_int(scene->shader, OB_TEXTURE_UNIFORMS[OB_Object_get_texture_slot(object)], (int)OB_Object_get_texture_slot(object));
        }
    }

    OB_VertexArrayObject_add_buffer(&scene->vao, &scene->vab, NULL, &OB_OBJECT_VERTEX_LAYOUT);
    OB_Shader_set_mat4_l(scene->shader, scene->projection_id,OB_PerspectiveCamera_get_projection(scene->perspective_camera));
}

void OB_Scene3D_end(struct OB_Scene3D *scene) {
    assert(scene != NULL);

    OB_VertexArrayObject_unbind(&scene->vao);
    OB_VertexArrayBuffer_free(&scene->vab);
    OB_VertexArrayBuffer_unbind(&scene->vab);
}

void OB_Scene3D_draw_objects(struct OB_Scene3D *scene) {
    assert(scene != NULL);

    OB_OpenGL_clear();

    OB_Lock_lock(&scene->perspective_camera->camera.lock);
    OB_Shader_set_mat4_l(scene->shader, scene->projection_id, OB_PerspectiveCamera_get_projection(scene->perspective_camera));
    OB_Shader_set_mat4_l(scene->shader, scene->view_id, OB_PerspectiveCamera_get_view(scene->perspective_camera));
    OB_Shader_set_vec3_l(scene->shader, scene->perspective_camera->camera.id, scene->perspective_camera->camera.position);
    OB_Lock_unlock(&scene->perspective_camera->camera.lock);

    int first = 0;
    if(scene->background != NULL) {
        OB_OpenGL_draw_background(first, (int)scene->background->vertices_count);
        first += (int)scene->background->vertices_count;
    }

    OB_OpenGL_draw(first, (int)scene->vertices_count);
}

void OB_Scene3D_update_objects(struct OB_Scene3D *scene) {
    assert(scene != NULL);

    OB_Lock_lock(&scene->vab.lock);
    for(unsigned i = 0u; i < (unsigned)scene->to_update.length; i++) {
        struct OB_SceneObject3D *const scene_object = &scene->scene_objects.buffer[scene->to_update.buffer[i]]; 
        OB_VertexArrayBuffer_set(&scene->vab, scene_object->offset * sizeof(*scene_object->object->vertices), scene_object->object->vertices, scene_object->object->vertices_count * sizeof(*scene_object->object->vertices));
        scene_object->needs_update = true;
    }
    scene->to_update.length = 0;
    OB_Lock_unlock(&scene->vab.lock);
}