#pragma once

#ifndef __FONT_HPP
#define __FONT_HPP

#include <memory>

struct SDL_Surface;
struct FontImpl;

class Font {
  public:
    Font(const char* filename, float ptsize, float xadvance_adjust = 0.0);
    ~Font();

    void Draw(SDL_Surface* surface, int x, int y, const char* text) const;

    int Height() const;
    int Baseline() const;
    int Width(const char* text) const;

  private:
    FontImpl* self;
};

#endif
