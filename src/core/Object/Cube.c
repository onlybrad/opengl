#include <assert.h>
#include "Cube.h"
#include "../Util/util.h"

static struct OB_Vertex background_vertices[] = OB_BLACK_CUBE_VERTICES(1.0f);
static struct OB_Vertex cube_vertices[] = OB_CUBE_VERTICES(0.5f, OB_CURLY_BRACKETS({1.0f, 0.5f, 0.31f, 1.0f}));

bool OB_Cube_create_background(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Object_init(object, background_vertices, (unsigned)OB_ARRAY_LEN(background_vertices))) {
        return false;
    }

    mat4 model;
    glm_mat4_identity(model);
    OB_Object_set_model(object, (float*)model);

    return true;
}

bool OB_Cube_create(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Object_init(object, cube_vertices, (unsigned)OB_ARRAY_LEN(cube_vertices))) {
        return false;
    }
    
    mat4 model;
    glm_mat4_identity(model);
    OB_Object_set_model(object, (float*)model);
    OB_Object_set_shininess(object, 32u);

    return true;
}

bool OB_Cube_create_light(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Cube_create(object)) {
        return false;
    }

    OB_Object_set_is_light(object, true);

    return true;
}
