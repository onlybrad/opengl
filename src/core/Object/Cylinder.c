#include <assert.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include "Cylinder.h"

static Vertex *generate_cylinder_vertices(const unsigned segments, const float radius, const float height, size_t *const size) {
    const float π = 3.141593f;
    const float step = 2.0f * π / (float)segments;

    *size = (size_t)segments * 6 * sizeof(Vertex);
    Vertex *vertices = (Vertex*)calloc((size_t)segments * 6, sizeof(Vertex));

    if(vertices == NULL) {
        return NULL;
    }
    
    for (unsigned i = 0u; i < segments; i++) {
        const float θ = (float)i * step;
        const float x1 = radius * cosf(θ);
        const float z1 = radius * sinf(θ);
        const float x2 = radius * cosf(θ + step);
        const float z2 = radius * sinf(θ + step);
        
        Vertex *vertex = vertices + i * 6u;

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

bool Cylinder_create(Object *object, const float radius, const float height) {
    assert(object != NULL);

    size_t size;
    Vertex *cylinder_vertices = generate_cylinder_vertices(20u, radius, height, &size);
    if(cylinder_vertices == NULL) {
        return false;
    }

    Object_init_move(object, cylinder_vertices, (unsigned)(size/sizeof(*cylinder_vertices)));
    mat4 model;
    glm_mat4_identity(model);
    Object_set_model(object, (float*)model);
    Object_set_shininess(object, 32u);

    return true;
}
