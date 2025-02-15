#include <cglm/cglm.h>
#include <stdlib.h>
#include "Cylinder.h"

static Vertex *generate_cylinder_vertices(const unsigned int segments, const float radius, const float height, size_t *const size) {
    const float π = 3.141593f;
    const float step = 2.0f * π / (float)segments;

    *size = (size_t)segments * 6 * sizeof(Vertex);
    Vertex *vertices = calloc((size_t)segments * 6, sizeof(Vertex));
    
    for (unsigned int i = 0u; i < segments; i++) {
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

void Cylinder_create(Object object[static 1], const float radius, const float height) {
    size_t size;
    Vertex *cylinder_vertices = generate_cylinder_vertices(20u, radius, height, &size);

    Object_init_move(object, cylinder_vertices, (unsigned int)(size/sizeof(*cylinder_vertices)));
    mat4 model;
    glm_mat4_identity(model);
    Object_set_model(object, (float*)model);
    Object_set_shininess(object, 32u);
}
