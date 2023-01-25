#pragma once

#ifndef __APP_HPP
#define __APP_HPP

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "window.hpp"
#include "defines.hpp"
#include "common.hpp"
#include "font.hpp"

#include "render-imgui.hpp"
#include "render-sprites.hpp"
#include "render-shapes.hpp"
#include "render-surface.hpp"

using namespace std;

class Game {
    public:
        Game();

        void Run();
        void Stop();
        void LoopCallback();

        bool shouldStop;
        bool running;

    private:
        void Setup();
        void Preload();

        Window* window;
        RenderSprites* spriteLayer;
        RenderShapes* shapeLayer;
};

#endif
