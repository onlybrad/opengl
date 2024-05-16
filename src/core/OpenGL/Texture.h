#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>

typedef enum TextureError {
    TextureNoError,
    TextureNotFound,
    TextureUnsupportedChannels
} TextureError ;

typedef struct Texture {
    GLuint id;
    const GLubyte *data;
    const char* name;
    GLint width;
    GLint height;
    GLint channels;
} Texture;

TextureError Texture_init(Texture *const texture, const char *const name, const GLchar *const path);
void Texture_free(Texture *const texture);
void Texture_use(const Texture *const texture, const GLuint slot);
void Texture_color(Texture *const texture, const unsigned char color[static 4]);

#endif