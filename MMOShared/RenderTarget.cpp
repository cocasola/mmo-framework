//
// Created by Keegan Beaulieu on 2024-07-09.
//

#include "RenderTarget.h"

#include <iostream>

#include <GL/glew.h>
#include <__format/format_functions.h>

#include "Window.h"

List<RenderTarget> RenderTarget::render_targets;

RenderTarget *RenderTarget::create(const Params &params)
{
    RenderTarget *target = render_targets.alloc();

    target->name = params.name;
    target->depth_buffer = params.depth_buffer;

    target->texture = Texture::create({
        .width = params.width,
        .height = params.height,
        .format = params.format,
        .filtering = params.filtering
    });

    glGenFramebuffers(1, &target->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, target->fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target->texture->texture, 0);

    if (params.depth_buffer) {
        glGenRenderbuffers(1, &target->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, target->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, params.width, params.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, target->rbo);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error(std::format("Failed to create RenderTarget '{}'. Failed to create framebuffer.", target->name));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return target;
}

void RenderTarget::destroy(RenderTarget *render_target)
{
    render_targets.destroy(render_target);
}

void RenderTarget::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, texture->width, texture->height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::resize(int width, int height)
{
    texture->resize(width, height);

    if (depth_buffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

void RenderTarget::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::true_size.x, Window::true_size.y);
}

void RenderTarget::read_pixel_imp(int x, int y, void *pixel)
{
    GLenum format = Texture::get_gl_format(texture->format);
    GLenum type = Texture::get_gl_type(texture->format);

    glFlush();
    glFinish();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadPixels(x, texture->height - y - 1, 1, 1, format, type, pixel);
}

void RenderTarget::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, texture->width, texture->height);
}
