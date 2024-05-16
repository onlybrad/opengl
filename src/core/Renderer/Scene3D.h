#ifndef _SCENE_H
#define _SCENE_H

#include <stdlib.h>
#include "Camera.h"
#include "../util.h"
#include "../OpenGL/VertexArrayBuffer.h"
#include "../OpenGL/VertexArrayObject.h"
#include "../OpenGL/VertexLayout.h"
#include "../OpenGL/Shader.h"
#include "../Object/Object1.h"

typedef struct Transform {
    float rotation_angle;
    vec3 rotation_axis;
    vec3 translate;
    vec3 scale;
} Transform;

typedef struct Scene3DObject1 {
    Object1 *object;
    Transform transform;
    unsigned int offset;
    bool needs_update;
} Scene3DObject1;

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
    Object1 *background;
    Scene3DObject1 scene_objects1[128];
    unsigned int to_update[128];
} Scene3D;

void Scene3D_init(Scene3D *const scene, Shader *const shader, PerspectiveCamera *const perspective_camera);
void Scene3D_free(Scene3D *const scene);
bool Scene3D_add_object1(Scene3D *const scene, Object1 *const object, const Transform *const transform);
void Scene3D_object1_set_transform(Scene3D *const scene, unsigned int object_index, Transform *const transform);
void Scene3D_set_background1(Scene3D *const scene, Object1 *const background);
Scene3DObject1 *Scene3D_object1_get(Scene3D *const scene, unsigned int object_index);
void Scene3D_start1(Scene3D *const scene);
void Scene3D_end1(Scene3D *const scene);
void Scene3D_update_objects1(Scene3D *const scene);
void Scene3D_draw_objects1(Scene3D *const scene);

#endif