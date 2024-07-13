//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef IMAGES_H
#define IMAGES_H

#include <memory>
#include <GLFW/glfw3.h>

struct ImageWithTexture {
    uint32_t *buffer;
    GLuint gl_texture;

    int width;
    int height;
};

auto create_image_with_texture(int image_width, int image_height) -> ImageWithTexture
{
    ImageWithTexture out;
    out.width = image_width;
    out.height = image_height;
    out.buffer = (uint32_t *) malloc(image_width * image_height * 4);

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &out.gl_texture);
    glBindTexture(GL_TEXTURE_2D, out.gl_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, out.buffer);

    return out;
};

void upload_texture(const ImageWithTexture &image)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.buffer);
}

void delete_image_with_texture(const ImageWithTexture &image)
{
    free(image.buffer);
    glDeleteTextures(1, &image.gl_texture);
}

#endif //IMAGES_H
