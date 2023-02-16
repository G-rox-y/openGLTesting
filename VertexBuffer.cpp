#include "VertexBuffer.hpp"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
    glGenBuffers(1, &m_RendererID); //generate the buffer and save its index
    this->Bind(); //bind so i can pass the data
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); //pass data to buffer
    this->Unbind(); //unbind
}

VertexBuffer::~VertexBuffer(){
    this->Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}