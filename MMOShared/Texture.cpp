//
// Created by Keegan Beaulieu on 2024-07-08.
//

#include "Texture.h"

#include <iostream>
#include <format>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

List<Texture> Texture::textures;

GLenum Texture::get_gl_format(Texture::Format format)
{
    switch (format) {
        case Texture::TEX_RED:
            return GL_RED;
        case Texture::TEX_RGB:
            return GL_RGB;
        case Texture::TEX_RGBA:
            return GL_RGBA;
        case Texture::TEX_32UI:
            return GL_RED_INTEGER;
    }
}

GLenum Texture::get_gl_internal_format(Texture::Format format)
{
    switch (format) {
        case Texture::TEX_RED:
            return GL_RED;
        case Texture::TEX_RGB:
            return GL_RGB;
        case Texture::TEX_RGBA:
            return GL_RGBA;
        case Texture::TEX_32UI:
            return GL_R32UI;
    }
}

GLenum Texture::get_gl_type(Texture::Format format)
{
    switch (format) {
        case Texture::TEX_RED:
            return GL_UNSIGNED_BYTE;
        case Texture::TEX_RGB:
            return GL_UNSIGNED_BYTE;
        case Texture::TEX_RGBA:
            return GL_UNSIGNED_BYTE;
        case Texture::TEX_32UI:
            return GL_UNSIGNED_INT;
    }
}

Texture *Texture::create(const Params &params)
{
    Texture *texture = textures.alloc();

    texture->name = params.name;
    texture->width = params.width;
    texture->height = params.height;
    texture->format = params.format;

    GLenum gl_filtering = GL_NEAREST;
    GLenum gl_internal_format = GL_RGB;
    GLenum gl_format = GL_RGB;
    GLenum gl_type = GL_UNSIGNED_BYTE;

    switch (params.filtering) {
        case TEX_LINEAR:
            gl_filtering = GL_LINEAR;
            break;
        case TEX_NEAREST:
            gl_filtering = GL_NEAREST;
            break;
    }

    unsigned char *pixels = params.pixels;

    if (params.path != "") {
        int nr_channels;
        stbi_set_flip_vertically_on_load(true);
        pixels = stbi_load(params.path.c_str(), &texture->width, &texture->height, &nr_channels, 0);

        if (pixels) {
            switch (nr_channels) {
                case 1:
                    gl_internal_format = GL_RED;
                    gl_format = GL_RED;
                    gl_type = GL_UNSIGNED_BYTE;
                    texture->format = TEX_RED;
                    break;
                case 3:
                    gl_internal_format = GL_RGB;
                    gl_format = GL_RGB;
                    gl_type = GL_UNSIGNED_BYTE;
                    texture->format = TEX_RGB;
                    break;
                case 4:
                    gl_internal_format = GL_RGBA;
                    gl_format = GL_RGBA;
                    gl_type = GL_UNSIGNED_BYTE;
                    texture->format = TEX_RGBA;
                    break;
            }
        } else {
            texture = 0;
            stbi_image_free(pixels);

            throw std::runtime_error(std::format("Failed to create texture '{}' at path '{}'.", texture->name, params.path));
        }
    } else {
        gl_format = get_gl_format(texture->format);
        gl_internal_format = get_gl_internal_format(texture->format);
        gl_type = get_gl_type(texture->format);

        if (params.invert && pixels != nullptr) {
            size_t row_size = params.stride*texture->width;
            unsigned char *buffer = (unsigned char *)malloc(row_size);

            for (int row = 0; row < params.height/2; ++row) {
                memcpy(buffer, pixels + row*row_size, row_size);
                memcpy(pixels + row*row_size, pixels + (texture->height - row - 1)*row_size, row_size);
                memcpy(pixels + (texture->height - row - 1)*row_size, buffer, row_size);
            }

            free(buffer);
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, params.stride);

    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filtering);

    glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, texture->width, texture->height, 0, gl_format, gl_type, pixels);

    if (pixels && params.path != "")
        stbi_image_free(pixels);

    return texture;
}

void Texture::destroy(Texture *texture)
{
    texture->cleanup_texture();
    textures.destroy(texture);
}

void Texture::cleanup()
{
    for (auto texture : textures) {
        texture.cleanup_texture();
    }
}

void Texture::cleanup_texture()
{
    glDeleteTextures(1, &texture);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::resize(int width, int height)
{
    GLenum gl_format = get_gl_format(format);
    GLenum gl_type = get_gl_type(format);
    GLenum gl_internal_format = get_gl_internal_format(format);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, width, height, 0, gl_format, gl_type, nullptr);

    this->width = width;
    this->height = height;
}
