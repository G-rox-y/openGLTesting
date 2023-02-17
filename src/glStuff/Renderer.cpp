#include "Renderer.hpp"

Renderer::Renderer(){}
Renderer::~Renderer(){}

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    // ^ draw triangles using the currently bound vertex array data while following the triangle indices pattern of
    // the index buffer (do it id.GetCount() times), all indices are unsigned ints at the offset of 0 from the 
    // beginning of the index buffer object

    shader.Unbind();
    va.Unbind();
    ib.Unbind();
}