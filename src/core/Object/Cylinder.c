#include <assert.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include "Cylinder.h"

static struct OB_Vertex *OB_generate_cylinder_vertices(unsigned segments, float radius, float height, size_t *size) {
    const float π = 3.141593f;
    const float step = 2.0f * π / (float)segments;

    *size = (size_t)segments * 6 * sizeof(struct OB_Vertex);
    struct OB_Vertex *vertices = (struct OB_Vertex*)calloc((size_t)segments * 6, sizeof(struct OB_Vertex));

    if(vertices == NULL) {
        return NULL;
    }
    
    for (unsigned i = 0u; i < segments; i++) {
        const float θ = (float)i * step;
        const float x1 = radius * cosf(θ);
        const float z1 = radius * sinf(θ);
        const float x2 = radius * cosf(θ + step);
        const float z2 = radius * sinf(θ + step);
        
        struct OB_Vertex *vertex = vertices + i * 6u;

        vertex->position[0] = x1;
        vertex->position[1] = height;
        vertex->position[2] = z1;
        vertex->texture_coordinate[0] = (float)i / (float)segments;
        vertex->texture_coordinate[1] = 0.0f;
        vertex->normal[0] = 0.0f;
        vertex->normal[1] = 1.0f;
        vertex->normal[2] = 0.0f;
        
        vertex++;

        vertex->position[0] = x2;
        vertex->position[1] = height;
        vertex->position[2] = z2;
        vertex->texture_coordinate[0] = (float)i / (float)segments;
        vertex->texture_coordinate[1] = 1.0f;
        vertex->normal[0] = 0.0f;
        vertex->normal[1] = 1.0f;
        vertex->normal[2] = 0.0f;
        
        vertex++;

        vertex->position[0] = x1;
        vertex->position[1] = -height;
        vertex->position[2] = z1;
        vertex->texture_coordinate[0] = (float)i / (float)segments;
        vertex->texture_coordinate[1] = 0.0f;
        vertex->normal[0] = 0.0f;
        vertex->normal[1] = -1.0f;
        vertex->normal[2] = 0.0f;

        vertex++;

        vertex->position[0] = x2;
        vertex->position[1] = height;
        vertex->position[2] = z2;
        vertex->texture_coordinate[0] = (float)i / (float)segments;
        vertex->texture_coordinate[1] = 1.0f;
        vertex->normal[0] = 0.0f;
        vertex->normal[1] = 1.0f;
        vertex->normal[2] = 0.0f;
        
        vertex++;

        vertex->position[0] = x2;
        vertex->position[1] = -height;
        vertex->position[2] = z2;
        vertex->texture_coordinate[0] = (float)i / (float)segments;
        vertex->texture_coordinate[1] = 0.0f;
        vertex->normal[0] = 0.0f;
        vertex->normal[1] = -1.0f;
        vertex->normal[2] = 0.0f;

        vertex++;

        vertex->position[0] = x1;
        vertex->position[1] = -height;
        vertex->position[2] = z1;
        vertex->texture_coordinate[0] = (float)i / (float)segments;
        vertex->texture_coordinate[1] = 1.0f;
        vertex->normal[0] = 0.0f;
        vertex->normal[1] = -1.0f;
        vertex->normal[2] = 0.0f;
    }

    return vertices;
}

bool OB_Cylinder_create(struct OB_Object *object, float radius, float height) {
    assert(object != NULL);

    size_t size;
    struct OB_Vertex *cylinder_vertices = OB_generate_cylinder_vertices(20u, radius, height, &size);
    if(cylinder_vertices == NULL) {
        return false;
    }

    OB_Object_init_move(object, cylinder_vertices, (unsigned)(size/sizeof(*cylinder_vertices)));
    mat4 model;
    glm_mat4_identity(model);
    OB_Object_set_model(object, (float*)model);
    OB_Object_set_shininess(object, 32u);

    return true;
}
