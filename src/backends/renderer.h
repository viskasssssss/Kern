// src/backends/renderer.h
#pragma once
#include "utils/vectors.h"
#include "utils/colors.h"

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void clear() = 0;
    virtual void present() = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void renderTri(kern::Vector2 a, kern::Vector2 b, kern::Vector2 c, kern::Color color) = 0;
    virtual void renderLine(kern::Vector2 a, kern::Vector2 b, kern::Color color, float thickness) = 0;
    virtual void renderCircle(kern::Vector2 center, float radius, kern::Color color) = 0;
};
