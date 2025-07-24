#include <assert.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Texture.h"
#include "../Util/util.h"

const char TEXTURE_UNIFORMS[32][10] = {
    "texture00",
    "texture01",
    "texture02",
    "texture03",
    "texture04",
    "texture05",
    "texture06",
    "texture07",
    "texture08",
    "texture09",
    "texture10",
    "texture11",
    "texture12",
    "texture13",
    "texture14",
    "texture15",
    "texture16",
    "texture17",
    "texture18",
    "texture19",
    "texture20",
    "texture21",
    "texture22",
    "texture23",
    "texture24",
    "texture25",
    "texture26",
    "texture27",
    "texture28",
    "texture29",
    "texture30",
    "texture31",
};

extern int MAX_TEXTURE_IMAGE_UNITS;
extern int MAX_COMBINED_TEXTURE_IMAGE_UNITS;

static const unsigned char DEFAULT_TEXTURE[4] = {0, 0, 0, 255}; //Opaque black

static void generate_texture(Texture *texture) {
    assert(texture != NULL);

    GLenum format;
    if(texture->channels == 3) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        format = GL_RGB;
    } else if(texture->channels == 4) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        format = GL_RGBA;
    } else {
        return;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture_init(Texture *texture, const char *name, const char *path) {
    assert(texture != NULL);
    assert(name != NULL);
    assert(path != NULL);

    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

    if(data == NULL) {
        fprintf(stderr, "Couldn't load texture at %s\n", path);
        Texture_color(texture, NULL, NULL);
        return;
    }

    texture->channels = channels;
    texture->width = width;
    texture->height = height;
    texture->data = data;
    texture->name = name;
    generate_texture(texture);
}

void Texture_free(Texture *texture) {
    assert(texture != NULL);

    stbi_image_free(texture->data);
    texture->data = NULL;
}

void Texture_use(const Texture *texture, const unsigned slot) {
    assert(texture != NULL);

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void Texture_color(Texture *texture, const char* name, const Color *color) {
    assert(texture != NULL);
    assert(name != NULL);
    assert(color != NULL);

    texture->channels = 4;
    texture->width = 1;
    texture->height = 1;

    const size_t size = 4 * sizeof(unsigned char);
    texture->data = malloc(size);
    memcpy(texture->data, color == NULL ? DEFAULT_TEXTURE : (const unsigned char*)color, size);
    
    texture->name = NULL ? "default" : name;
    generate_texture(texture);
}