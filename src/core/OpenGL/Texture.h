#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include "../Util/Color.h"

typedef struct Texture {
    unsigned int id;
    const unsigned char *data;
    const char *name;
    int width;
    int height;
    int channels;
} Texture;

void Texture_init(Texture *const texture, const char *const name, const char *const path);
void Texture_free(Texture *const texture);
void Texture_use(const Texture *const texture, const unsigned int slot);
void Texture_color(Texture *const texture, const char* const name, Color *const color);

#endif