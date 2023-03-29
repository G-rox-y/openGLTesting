#pragma once

#include <GL/glew.h>
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned int normalized; //(bool)

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type){
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        //code error here
        return 0;
    }

    static unsigned int GetNormalizationOfType(unsigned int type)
    {
        switch (type){
            case GL_FLOAT: return 0;
            case GL_UNSIGNED_INT: return 0;
            case GL_UNSIGNED_BYTE: return 1;
        }
        //code error here
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout() : m_Stride(0){};
    ~VertexBufferLayout(){};

    void Push(unsigned int type, unsigned int count)
    {
        bool norm = VertexBufferElement::GetNormalizationOfType(type);
        VertexBufferElement v = {type, count, norm};
        m_Elements.emplace_back(v);
        m_Stride += count * VertexBufferElement::GetSizeOfType(type);
    }

    inline const std::vector<VertexBufferElement>& GetElements() const {return m_Elements;}
    inline unsigned int GetStride() const {return m_Stride;}

};