#pragma once

#include <GL/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
private:
    int display_w, display_h;
public:
    Renderer();
    Renderer(int W, int H);
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Resize(const int& width, const int& height);

    inline const float Width() const { return display_w; };
    inline const float Height() const { return display_h; };
};