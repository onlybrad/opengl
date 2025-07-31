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
#include "../template/vector.h"

//Vector_uint
typedef unsigned uint;
#define VECTOR_T uint
#include "../template/vector.h"

typedef struct OB_Scene3D {
    int projection_id;                         //projection location id for glsl, used to update the projection matrix
    int view_id;                               //view location id for glsl, used to update the view matrix
    unsigned texture_slot;                     //the last unused texture slot, used for binding textures to objects
    unsigned vertices_count;                   //the number of vertices in the scene, used for drawing in opengl
    struct OB_VertexArrayObject vao;  
    struct OB_VertexArrayBuffer vab;
    struct OB_Camera *camera;                  //the camera associated with this scene
    struct OB_Shader *shader;                  //the vertex and fragment shader associated with this scene
    struct OB_Object *background;              //the background to be rendered in the scene
    struct Vector_SceneObject3D scene_objects; //all the objects in the scene
    struct Vector_uint to_update;              //whether or not the object at index i needs to be updated in the vertex buffer
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