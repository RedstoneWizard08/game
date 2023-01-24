#pragma once

#ifndef __ATLAS_HPP
#define __ATLAS_HPP

#include <memory>

struct SDL_Surface;
struct AtlasImpl;

struct SpriteLocation {
    float x0, y0, x1, y1;
    float s0, t0, s1, t1;
};

class Atlas {
  public:
    Atlas();
    ~Atlas();

    int LoadImage(const char* filename);
    int AddSurface(SDL_Surface* surface);

    SDL_Surface* GetSurface();

    const SpriteLocation& GetLocation(int id) const;

  private:
    std::unique_ptr<AtlasImpl> self;
};

#endif
