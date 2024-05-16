#ifndef _opengl_GLFW_H
#define _WINDOW_GLFW_H

#include <glad/glad.h>

void OpenGL_init(GLADloadproc proc_address, void (terminate)(void));
void OpenGL_set_width(const GLint width);
void OpenGL_set_height(const GLint height);
void OpenGL_set_viewport(const GLint width, const GLint height);
void OpenGL_clear_color(const float r, const float g, const float b, const float a);
void OpenGL_clear(void);
void OpenGL_fill_mode(void);
void OpenGL_wireframe_mode(void);
void OpenGL_draw_background(const int first, const int vertices_count);
void OpenGL_draw(const int first, const int vertices_count);
void OpenGL_print_errors(void);

#endif