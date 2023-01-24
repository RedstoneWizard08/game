#pragma once

#ifndef __RENDER_IMGUI_HPP
#define __RENDER_IMGUI_HPP

#include "render-layer.hpp"
#include <memory>

struct SDL_Window;
struct RenderImGuiImpl;

class RenderImGui : public IRenderLayer {
  public:
    RenderImGui();
    ~RenderImGui();
    virtual void Render(SDL_Window* window, bool reset);
    virtual void ProcessEvent(SDL_Event* event);

  protected:
    std::unique_ptr<RenderImGuiImpl> self;
};

#endif
