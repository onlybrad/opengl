#include <stdio.h>
#include <stdlib.h>
#include "OpenGL.h"
#include "../util.h"
#include "../OpenGL/VertexLayout.h"
#include "../Object/Object1.h"

GLint OpenGL_width;
GLint OpenGL_height;
GLint MAX_VERTEX_ATTRIBS;
GLint MAX_COMBINED_TEXTURE_IMAGE_UNITS;
GLint MAX_TEXTURE_IMAGE_UNITS;
VertexLayout OBJECT_VERTEX_LAYOUT;

static void opengl_defaults_init(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

static void max_values_init(void) {
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MAX_VERTEX_ATTRIBS);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_IMAGE_UNITS);
}

static void vertex_layout_init(void) {
    VertexAttribute attributes[] = {
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, position)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, normal)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, color)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, texture_coordinate)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, texture_slot)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, is_light)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, model_1)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, model_2)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, model_3)},
        {.count = ARRAY_MEMBER_LENGTH(Vertex1, model_4)}
    };

    VertexLayout_init(&OBJECT_VERTEX_LAYOUT);

    for(int i=0; i<(int)ARRAY_LENGTH(attributes); i++) {
        VertexLayout_push(&OBJECT_VERTEX_LAYOUT, &attributes[i]);
    }
}

void OpenGL_init(GLADloadproc proc_address, void (terminate)(void)) {
    if (! gladLoadGLLoader(proc_address)) {
        fputs("Failed to load OpenGL functions",stderr);
        terminate();
        exit(-1);
    }

    opengl_defaults_init();
    max_values_init();
    vertex_layout_init();
}

inline void OpenGL_set_width(const GLint width) {
    OpenGL_width = width;
    glViewport(0, 0, width, OpenGL_height);
}

inline void OpenGL_set_height(const GLint height) {
    OpenGL_height = height;
    glViewport(0, 0, OpenGL_width, height);    
}

inline void OpenGL_set_viewport(const GLint width, const GLint height) {
    OpenGL_width = width;
    OpenGL_height = height;
    glViewport(0, 0, width, height);   
}

inline void OpenGL_clear_color(const float r, const float g, const float b, const float a) {
    glClearColor(r, g, b, a);
}

inline void OpenGL_clear(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void OpenGL_fill_mode(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void OpenGL_wireframe_mode(void) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

inline void OpenGL_draw_background(const int first, const int vertices_count) {
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, first, vertices_count);
    glEnable(GL_DEPTH_TEST);
}

inline void OpenGL_draw(const int first, const int vertices_count) {
    glDrawArrays(GL_TRIANGLES, first, vertices_count);
}

void OpenGL_print_errors(void) {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error: %i", err);
    }
}