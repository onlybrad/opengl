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

extern GLint MAX_TEXTURE_IMAGE_UNITS;
extern GLint MAX_COMBINED_TEXTURE_IMAGE_UNITS;

static const unsigned char DEFAULT_TEXTURE[4] = {0, 0, 0, 255}; //Opaque black

static void generate_texture(Texture *const texture) {
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

TextureError Texture_init(Texture *const texture, const char *const name, const char *const path) {
    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    const unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

    if(data == NULL) {
        fprintf(stderr, "Couldn't load texture at %s\n", path);
        Texture_color(texture, NULL, NULL);
        return TextureNotFound;
    }

    texture->channels = channels;
    texture->width = width;
    texture->height = height;
    texture->data = data;
    texture->name = name;
    generate_texture(texture);

    return TextureNoError;
}

void Texture_free(Texture *const texture) {
    stbi_image_free(unconst(texture->data));
    texture->data = NULL;
}

void Texture_use(const Texture *const texture, const GLuint slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void Texture_color(Texture *const texture, const char* const name, Color *const color) {
    texture->channels = 4;
    texture->width = 1;
    texture->height = 1;

    const size_t size = 4 * sizeof(unsigned char);
    texture->data = malloc(size);
    memcpy(unconst(texture->data), color == NULL ? DEFAULT_TEXTURE : (unsigned char*)color, size);
    
    texture->name = NULL ? "default" : name;
    generate_texture(texture);
}