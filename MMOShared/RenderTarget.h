//
// Created by Keegan Beaulieu on 2024-07-09.
//

#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "Texture.h"

class RenderTarget
{
public:
    struct Params
    {
        std::string name = "Render Target";

        int width = 512;
        int height = 512;

        bool depth_buffer = true;

        Texture::Format format = Texture::TEX_RGB;
        Texture::Filtering filtering = Texture::TEX_NEAREST;
    };

    Texture *texture;
    std::string name;
    bool depth_buffer;

    void bind();
    void clear();
    void resize(int width, int height);

    template<class T>
    T read_pixel(int x, int y);

    static RenderTarget *create(const Params &params);
    static void destroy(RenderTarget *render_target);
    static void unbind();

    void read_pixel_imp(int x, int y, void *pixel);

private:
    static List<RenderTarget> render_targets;

    unsigned int fbo;
    unsigned int rbo;
};


template<typename T>
T RenderTarget::read_pixel(int x, int y)
{
    T data;
    read_pixel_imp(x, y, &data);

    return data;
}

#endif //RENDERTARGET_H
