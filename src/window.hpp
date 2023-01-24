#pragma once

#include <SDL2/SDL_events.h>
#ifndef __WINDOW_HPP
#define __WINDOW_HPP

#include <memory>
#include <SDL2/SDL.h>

#include "glwrappers.hpp"
#include "render-layer.hpp"

using namespace std;

struct WindowImpl;

class Window {
    public:
        Window(int width = 512, int height = 512);
        ~Window();
    
        void Render();
        void OnResize();
        void AddLayer(IRenderLayer* layer);
        void ProcessEvent(SDL_Event* event);

        static int FRAME;
        bool visible;
        int width, height;

    private:
        WindowImpl* self;
};

#endif
