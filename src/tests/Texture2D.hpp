#pragma once

#include <glm/glm.hpp>

#include "Test.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

namespace test
{

    class Texture2D : public Test
    {
    private:
        glm::vec3 m_translationA, m_translationB;
        glm::mat4 m_proj, m_view;

        VertexArray va;
        IndexBuffer ib;
        Renderer& renderer;
        Shader shader;
        Texture texture;
    public:
        Texture2D(Renderer& renderer, int& display_w, int& display_h);
        ~Texture2D() override;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImguiRender() override;
    };

}