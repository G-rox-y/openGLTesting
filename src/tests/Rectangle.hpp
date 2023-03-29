#pragma once

#include "Test.hpp"
#include "Renderer.hpp"

namespace test
{

    class Rectangle : public Test
    {
    private:
        float m_Points[8]; //vertex positions
        float m_Translation[2];
        float m_Scale;
        unsigned int m_Indices[6]; //data for index buffer

        // opengl objects
        VertexArray va;
        IndexBuffer ib;
        Shader shader;
        Renderer& renderer;
    public:
        Rectangle(Renderer& renderer);
        ~Rectangle() override;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImguiRender() override;
    };    
   
}
