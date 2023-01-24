#pragma once

#ifndef __WINDOW_CPP
#define __WINDOW_CPP

#include "window.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <SDL2/SDL_video.h>
#include <vector>

using namespace std;

struct WindowImpl {
    SDL_Window* window;
    bool initialized;
    GlContext* context;
    vector<IRenderLayer*> layers;

    WindowImpl(SDL_Window* window_);
    ~WindowImpl();
};

int Window::FRAME = 0;

Window::Window(int width_, int height_)
    : visible(true), width(width_), height(height_),
      self(new WindowImpl(
          SDL_CreateWindow("Hello, world!", SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED, this->width, this->height,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))) {
    this->OnResize();
}

Window::~Window() {}

void Window::AddLayer(IRenderLayer* layer) {
    this->self->layers.push_back(layer);
}

void Window::OnResize() {
    this->self->initialized = true;

    SDL_GL_GetDrawableSize(this->self->window, &this->width, &this->height);

    glViewport(0, 0, this->width, this->height);
}

void Window::Render() {
    if (this - this->visible) {
        glClear(GL_COLOR_BUFFER_BIT);
        for (auto layer : this->self->layers) {
            layer->Render(this->self->window, !this->self->initialized);
        }
        self->initialized = true;
        SDL_GL_SwapWindow(this->self->window);
        Window::FRAME++;
    }
}

void Window::ProcessEvent(SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT) {
        switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN: {
            this->visible = true;
            break;
        }
        case SDL_WINDOWEVENT_HIDDEN: {
            this->visible = false;
            break;
        }
        case SDL_WINDOWEVENT_SIZE_CHANGED: {
            this->OnResize();
            break;
        }
        }
    }

    for (auto layer : this->self->layers) {
        layer->ProcessEvent(event);
    }
}

WindowImpl::WindowImpl(SDL_Window* window_)
    : window(window_), initialized(false),
      context(new GlContext(this->window)) {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

WindowImpl::~WindowImpl() { SDL_DestroyWindow(window); }

#endif
