#include "VertexArray.hpp"

VertexArray::VertexArray(){
    glGenVertexArrays(1, &m_RendererID);
}
VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
    this->Bind();
    vb.Bind();
    
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for(int i = 0; i < elements.size(); i++){
        const auto& element = elements[i];

        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)0);
        // ^ explaining the data in the buffer
        // works as following: location 0 of VAO (currently bound vertex array) will be filled with 2 pairs of floats
        // without normalisation, each pair will have size of two sizeof(coords[0])
        // and gpu should search for them at the offset of 0 from the currently bound GL_ARRAY_BUFFER
        glEnableVertexAttribArray(0); // it has to be enabled manualy for some reason

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }


    vb.Unbind();
    this->Unbind();
}