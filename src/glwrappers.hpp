#pragma once

#ifndef __GLWRAPPERS_HPP
#define __GLWRAPPERS_HPP

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include "common.hpp"

void GLERRORS(const char* label);

SDL_Surface* CreateRGBASurface(int width, int height);

struct ShaderProgram : nocopy {
    GLuint id;
    ShaderProgram(const char* vertex_shader, const char* fragment_shader);
    ~ShaderProgram();

  private:
    void AttachShader(GLenum type, const GLchar* source);
};

struct Texture : nocopy {
    GLuint id;
    Texture(SDL_Surface* surface = nullptr);
    ~Texture();
    void CopyFromPixels(int width, int height, GLenum format, void* pixels);
    void CopyFromSurface(SDL_Surface* surface);
};

struct VertexBuffer : nocopy {
    GLuint id;
    VertexBuffer();
    ~VertexBuffer();
};

struct GlContext : nocopy {
    SDL_GLContext id;
    GlContext(SDL_Window* window);
    ~GlContext();
};

#endif
