#pragma once

#ifndef __RENDER_SHAPES_HPP
#define __RENDER_SHAPES_HPP

#include "render-layer.hpp"
#include <memory>
#include <vector>

struct SDL_Window;
struct RenderShapesImpl;

struct Triangle {
    float x[3], y[3];
};

struct Shape {
    vector<Triangle> triangles;
    float r, g, b, a;
};

class RenderShapes : public IRenderLayer {
  public:
    RenderShapes();
    ~RenderShapes();
    virtual void Render(SDL_Window* window, bool reset);

    void SetShapes(const vector<Shape>& shapes);

  protected:
    RenderShapesImpl* self;
};

#endif
