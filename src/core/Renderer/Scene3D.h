#ifndef _SCENE_H
#define _SCENE_H

#include <stdlib.h>
#include "Camera.h"
#include "../Util/util.h"
#include "../OpenGL/VertexArrayBuffer.h"
#include "../OpenGL/VertexArrayObject.h"
#include "../OpenGL/VertexLayout.h"
#include "../OpenGL/Shader.h"
#include "../Object/Object.h"

typedef struct Transform {
    float rotation_angle;
    vec3 rotation_axis;
    vec3 translate;
    vec3 scale;
} Transform;

typedef struct Scene3DObject {
    Object *object;
    Transform transform;
    unsigned int offset;
    bool needs_update;
} Scene3DObject;

typedef struct Scene3D {
    int projection_id;
    int view_id;
    unsigned int texture_slot;
    unsigned int objects_count;
    unsigned int objects_vertices_count;
    unsigned int to_update_count;
    VertexArrayObject vao;
    VertexArrayBuffer vab;
    PerspectiveCamera *perspective_camera;
    Shader *shader;
    Object *background;
    Scene3DObject scene_objects[128];
    unsigned int to_update[128];
} Scene3D;

void Scene3D_init(Scene3D *const scene, Shader *const shader, PerspectiveCamera *const perspective_camera);
void Scene3D_free(Scene3D *const scene);
bool Scene3D_add_object(Scene3D *const scene, Object *const object, const Transform *const transform);
void Scene3D_object_set_transform(Scene3D *const scene, const unsigned int object_index, Transform *const transform);
void Scene3D_object_needs_update(Scene3D *const scene, const unsigned int object_index);
void Scene3D_set_background1(Scene3D *const scene, Object *const background);
Scene3DObject *Scene3D_object_get(Scene3D *const scene, const unsigned int object_index);
void Scene3D_start1(Scene3D *const scene);
void Scene3D_end1(Scene3D *const scene);
void Scene3D_update_objects(Scene3D *const scene);
void Scene3D_draw_objects(Scene3D *const scene);

#endif