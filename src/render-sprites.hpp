#pragma once

#ifndef __RENDER_SPRITES_HPP
#define __RENDER_SPRITES_HPP

#include "render-layer.hpp"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

struct RenderSpritesImpl;

struct Sprite {
    int image_id;
    float x, y, rotation_degrees, scale;
};

class RenderSprites : public IRenderLayer {
  public:
    RenderSprites();
    ~RenderSprites();
    virtual void Render(SDL_Window* window, bool reset);

    void SetSprites(const std::vector<Sprite>& sprites);

  protected:
    RenderSpritesImpl* self;
};

#endif
