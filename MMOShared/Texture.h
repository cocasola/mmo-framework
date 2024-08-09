//
// Created by Keegan Beaulieu on 2024-07-08.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

#include "List.h"

class Texture
{
public:
    enum Format
    {
        TEX_RGB,
        TEX_RGBA,
        TEX_RED,
        TEX_32UI
    };

    enum Filtering
    {
        TEX_LINEAR,
        TEX_NEAREST
    };

    struct Params
    {
        std::string path = "";
        std::string name = "Texture";

        int width = 512;
        int height = 512;
        unsigned char *pixels = nullptr;
        bool invert = false;
        int stride = 4;

        Format format = TEX_RGB;
        Filtering filtering = TEX_LINEAR;
    };

    std::string name;

    int width;
    int height;

    Format format;
    Filtering filtering;

    unsigned int texture;

    void bind();
    void resize(int width, int height);

    static Texture *create(const Params &params);
    static void destroy(Texture *texture);
    static void cleanup();
    static GLenum get_gl_format(Format format);
    static GLenum get_gl_internal_format(Format format);
    static GLenum get_gl_type(Format format);

private:
    static List<Texture> textures;

    void cleanup_texture();
    void read_pixel_imp(int x, int y, void *pixel);
};

#endif //TEXTURE_H
