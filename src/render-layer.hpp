#pragma once

#ifndef __RENDER_LAYER_HPP
#define __RENDER_LAYER_HPP

#include <SDL2/SDL.h>
#include "common.hpp"

using namespace std;

struct IRenderLayer: nocopy {
  virtual void Render(SDL_Window* window, bool reset) {}
  virtual void ProcessEvent(SDL_Event* event) {}
  virtual ~IRenderLayer() {};
};


#endif
