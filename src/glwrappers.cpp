#pragma once

#include <cstdio>
#ifndef __GLWRAPPERS_CPP
#define __GLWRAPPERS_CPP

#include "glwrappers.hpp"
#include "common.hpp"

void GLERRORS(const char* label) {
#ifndef __EMSCRIPTEN__
    while (true) {
        GLenum err = glGetError();

        if (err == GL_NO_ERROR) {
            break;
        }
        
        char* erro;

        sprintf(erro, "%s glGetError returned %s\n", label, err);

        perror(erro);
    }
#endif
}

void FAIL(const char* label) {
    GLERRORS(label);

    char* err;

    sprintf(err, "%s failed : %s\n", label, SDL_GetError());

    perror(err);

    exit(EXIT_FAILURE);
}

SDL_Surface* CreateRGBASurface(int width, int height) {
    SDL_Surface* surface =
        SDL_CreateRGBSurface(0, width, height, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                             0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
                              0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
        );

    if (surface == nullptr) {
        FAIL("SDL_CreateRGBSurface");
    }

    return surface;
}

ShaderProgram::ShaderProgram(const char* vertex_shader,
                             const char* fragment_shader) {
    this->id = glCreateProgram();

    if (this->id == 0) {
        FAIL("glCreateProgram");
    }

#ifdef __EMSCRIPTEN__
    string new_fragment_shader = "precision highp float;\n";
    new_fragment_shader += fragment_shader;
    fragment_shader = new_fragment_shader.c_str();
#endif

    this->AttachShader(GL_VERTEX_SHADER, vertex_shader);
    this->AttachShader(GL_FRAGMENT_SHADER, fragment_shader);
    
    glLinkProgram(this->id);

    GLint link_status;

    glGetProgramiv(this->id, GL_LINK_STATUS, &link_status);

    if (!link_status) {
        GLint log_length;
    
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &log_length);
    
        GLchar log[1024];
    
        glGetProgramInfoLog(this->id, 1024, nullptr, log);
    
        char* err;
        sprintf(err, "GL Error (shader linking): %s\n", log);
        perror(err);
    
        FAIL("link shaders");
    }
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(this->id); }

void ShaderProgram::AttachShader(GLenum type, const GLchar* source) {
    GLuint shader_id = glCreateShader(type);

    if (shader_id == 0) {
        FAIL("load shader");
    }

    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);

    GLint compile_status;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
    
    if (!compile_status) {
        GLint log_length;

        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        GLchar log[1024];

        glGetShaderInfoLog(shader_id, 1024, nullptr, log);

        char* err;
        sprintf(err, "GL Error (shader compilation): %s\n", log);
        perror(err);

        FAIL("compile shader");
    }

    glAttachShader(this->id, shader_id);
    glDeleteShader(shader_id);

    GLERRORS("AttachShader()");
}

Texture::Texture(SDL_Surface* surface) {
    glGenTextures(1, &id);

    if (surface != nullptr) {
        this->CopyFromSurface(surface);
    }
}

void Texture::CopyFromPixels(int width, int height, GLenum format,
                             void* pixels) {
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
                 GL_UNSIGNED_BYTE, pixels);

    GLERRORS("Texture creation");
}

void Texture::CopyFromSurface(SDL_Surface* surface) {
    this->CopyFromPixels(surface->w, surface->h,
                   surface->format->BytesPerPixel == 1 ? GL_ALPHA
                   : surface->format->BytesPerPixel == 3
                       ? GL_RGB
                       : GL_RGBA,
                   surface->pixels);
}

Texture::~Texture() { glDeleteTextures(1, &this->id); }

VertexBuffer::VertexBuffer() { glGenBuffers(1, &this->id); }

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &this->id); }

GlContext::GlContext(SDL_Window* window) {
    this->id = SDL_GL_CreateContext(window);
    
    if (this->id == nullptr) {
        FAIL("SDL_GL_CreateContext");
    }
}

GlContext::~GlContext() { SDL_GL_DeleteContext(this->id); }

#endif
