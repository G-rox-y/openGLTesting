#include "IndexBuffer.hpp"

#include <GL/glew.h>

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_RendererID); //generate the buffer and save its index
}

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
: m_Count(count)
{
    glGenBuffers(1, &m_RendererID); //generate the buffer and save its index
    this->Bind(); //bind so i can pass the data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW); //pass data to buffer
    this->Unbind(); //unbind
}

IndexBuffer::~IndexBuffer()
{
    this->Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::AddData(const void* data, unsigned int count)
{
    m_Count = count;
    this->Bind(); //bind so i can pass the data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW); //pass data to buffer
    this->Unbind(); //unbind
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}