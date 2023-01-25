#pragma once

#ifndef __RENDER_SURFACE_HPP
#define __RENDER_SURFACE_HPP

#include "render-layer.hpp"
#include <memory>

struct SDL_Window;
struct SDL_Surface;
struct RenderSurfaceImpl;

class RenderSurface : public IRenderLayer {
  public:
    RenderSurface(SDL_Surface* surface);
    ~RenderSurface();
    virtual void Render(SDL_Window* window, bool reset);

  protected:
    RenderSurfaceImpl* self;
};

#endif
