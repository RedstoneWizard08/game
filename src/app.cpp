#pragma once

#ifndef __APP_CPP
#define __APP_CPP

#include "app.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

Triangle tri(float x1, float y1, float x2, float y2, float x3, float y3) {
    return Triangle{{x1, x2, x3}, {y1, y2, y3}};
}

Game::Game() {
    printf("Preloading...");

    this->Preload();

    printf("Setting up...");

    this->Setup();
}

void Game::LoopCallback() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            this->Stop();
            break;

        case SDL_KEYUP:
            int sym = event.key.keysym.sym;

            if (sym == SDLK_ESCAPE)
                this->Stop();

            break;
        }
    }

    if (this->window->visible) {
        static float t = 0.0;
        t += 0.1f;

#if SHOW_SPRITES
        {
            vector<Sprite> sprites;
            int SIDE = 4;
            int NUM = SIDE * SIDE;

            for (int j = 0; j < NUM; j++) {
                sprites.emplace_back();

                auto& s = sprites.back();

                s.image_id = 0;

                s.x = (0.5f + j % SIDE - 0.5f * SIDE + ((j / SIDE) % 2) * 0.5f -
                       0.25f) *
                      2.0f / SIDE;

                s.y = (0.5f + ((float)j) / SIDE - 0.5f * SIDE) * 2.0f / SIDE;

                s.scale = 2.0f / SIDE;
                s.rotation_degrees = j * 0.03f * t * DEG_TO_RAD;
            }

            this->spriteLayer->SetSprites(sprites);
        }
#endif

#if SHOW_SHAPES
        {
            vector<Shape> shapes;
            Shape s;

            s.r = 1.0;
            s.g = 0.5;
            s.b = 0.5;
            s.a = 1.0;

            s.triangles = {
                tri(0, -3, 1, -1, 0.5, 0),  tri(1, -1, 3, 0, 0.5, 0),
                tri(3, 0, 1, 1, 0.5, 0),    tri(1, 1, 0, 3, 0.5, 0),
                tri(0, 3, 0, 1, 0.5, 0),    tri(0, 1, -1, 1, 0.5, 0),
                tri(-1, 1, -1, -1, 0.5, 0), tri(-1, -1, 0, -1, 0.5, 0),
                tri(0, -1, 0, -3, 0.5, 0)};

            shapes.push_back(s);
            this->shapeLayer->SetShapes(shapes);
        }
#endif

        this->window->Render();
    }
}

void Game::Setup() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        FAIL("SDL_Init");
    }

    SDL_GL_SetSwapInterval(1);

    this->window = new Window(800, 600);

    Font font("imgui/misc/fonts/DroidSans.ttf", 32);

    SDL_Surface* overlay_surface =
        CreateRGBASurface(window->width, window->height);

    SDL_Rect fillarea;

    fillarea.x = 0;
    fillarea.y = 0;
    fillarea.w = 2 + font.Width("Hello world");
    fillarea.h = font.Height();

    SDL_FillRect(overlay_surface, &fillarea,
                 SDL_MapRGBA(overlay_surface->format, 64, 32, 0, 192));

    font.Draw(overlay_surface, 1, font.Baseline(), "Hello world");

#if SHOW_SPRITES
    this->spriteLayer = new RenderSprites;
    window->AddLayer(this->spriteLayer);
#endif

#if SHOW_SHAPES
    this->shapeLayer = new RenderShapes;
    window->AddLayer(this->shapeLayer);
#endif

#if SHOW_OVERLAY
    RenderSurface overlay_layer(overlay_surface);

    window->AddLayer(&overlay_layer);
#endif

#if SHOW_IMGUI
    RenderImGui ui_layer;

    window->AddLayer(&ui_layer);
#endif
}

void main_loop(void* obj) {
    Game* game = static_cast<Game*>(obj);

    if (!game->running)
        return;

    if (game->shouldStop) {
        game->Stop();
        return;
    }

    game->LoopCallback();
}

void Game::Run() {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, this, 0, 1);
#else
    while (this->running) {
        main_loop(this);
        SDL_Delay(16);
    }
#endif
}

void Game::Stop() {
    this->running = false;
    this->shouldStop = false;

#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif

    this->spriteLayer = nullptr;
    this->shapeLayer = nullptr;
    this->window = nullptr;

    SDL_Quit();
}

void Game::Preload() {}

#endif
