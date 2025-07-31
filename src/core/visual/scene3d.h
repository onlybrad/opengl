#ifndef OB_SCENE_H
#define OB_SCENE_H

#include <stdlib.h>
#include "camera.h"
#include "../util/util.h"
#include "../opengl/vertexarraybuffer.h"
#include "../opengl/vertexarrayobject.h"
#include "../opengl/vertexlayout.h"
#include "../opengl/shader.h"
#include "../object/object.h"



struct OB_Transform {
    float rotation_angle;
    vec3 rotation_axis;
    vec3 translate;
    vec3 scale;
};

struct OB_SceneObject3D {
    struct OB_Object *object;
    struct OB_Transform transform;
    unsigned offset;
    bool needs_update;
};

//Vector_SceneObject3D
typedef struct OB_SceneObject3D SceneObject3D;
#define VECTOR_T SceneObject3D
#include "../Template/Vector.h"

//Vector_uint
typedef unsigned uint;
#define VECTOR_T uint
#include "../Template/Vector.h"

typedef struct OB_Scene3D {
    int projection_id;
    int view_id;
    unsigned texture_slot;
    unsigned vertices_count;
    struct OB_VertexArrayObject vao;
    struct OB_VertexArrayBuffer vab;
    struct OB_Camera *camera;
    struct OB_Shader *shader;
    struct OB_Object *background;
    struct Vector_SceneObject3D scene_objects;
    struct Vector_uint to_update;
} Scene3D;

bool OB_Scene3D_init(struct OB_Scene3D *scene, struct OB_Shader *shader, struct OB_Camera *camera);
void OB_Scene3D_free(struct OB_Scene3D *scene);
bool OB_Scene3D_add_object(struct OB_Scene3D *scene, struct OB_Object *object, const struct OB_Transform *transform);
void OB_Scene3D_object_set_transform(struct OB_Scene3D *scene, unsigned object_index, struct OB_Transform *transform);
void OB_Scene3D_object_needs_update(struct OB_Scene3D *scene, unsigned object_index);
void OB_Scene3D_set_background(struct OB_Scene3D *scene, struct OB_Object *background);
struct OB_SceneObject3D *OB_Scene3D_object_get(struct OB_Scene3D *scene, unsigned object_index);
void OB_Scene3D_start(struct OB_Scene3D *scene);
void OB_Scene3D_end(struct OB_Scene3D *scene);
void OB_Scene3D_update_objects(struct OB_Scene3D *scene);
void OB_Scene3D_draw_objects(struct OB_Scene3D *scene);

#endif