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

typedef unsigned uint;
typedef struct Transform {
    float rotation_angle;
    vec3 rotation_axis;
    vec3 translate;
    vec3 scale;
} Transform;

typedef struct SceneObject3D {
    Object *object;
    Transform transform;
    unsigned offset;
    bool needs_update;
} SceneObject3D;

#define T SceneObject3D
#include "../Template/Vector.h"

#define T uint
#include "../Template/Vector.h"
typedef struct Scene3D {
    int projection_id;
    int view_id;
    unsigned texture_slot;
    unsigned vertices_count;
    VertexArrayObject vao;
    VertexArrayBuffer vab;
    PerspectiveCamera *perspective_camera;
    Shader *shader;
    Object *background;
    Vector_SceneObject3D scene_objects;
    Vector_uint to_update;
} Scene3D;

void Scene3D_init(Scene3D *scene, Shader *shader, PerspectiveCamera  *perspective_camera);
void Scene3D_free(Scene3D *scene);
bool Scene3D_add_object(Scene3D *scene, Object *object, const Transform *transform);
void Scene3D_object_set_transform(Scene3D *scene, const unsigned object_index, Transform *transform);
void Scene3D_object_needs_update(Scene3D *scene, const unsigned object_index);
void Scene3D_set_background(Scene3D *scene, Object *background);
SceneObject3D *Scene3D_object_get(Scene3D *scene, const unsigned object_index);
void Scene3D_start(Scene3D *scene);
void Scene3D_end(Scene3D *scene);
void Scene3D_update_objects(Scene3D *scene);
void Scene3D_draw_objects(Scene3D *scene);

#endif