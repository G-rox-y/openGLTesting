#include "Rectangle.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

namespace test
{
    Rectangle::Rectangle(Renderer& renderer)
    : m_Scale(10.f), m_Translation{100.f, 100.f}, 
        m_Points{
            -1.f, 1.f, 
            1.f, 1.f, 
            1.f, -1.f, 
            -1.f, -1.f,
        },
        m_Indices{
            0, 1, 2,
            2, 3, 0,
        },
        ib(m_Indices, 6), shader("resources/shaders/Rectangle.glsl"), renderer(renderer)
    {
        this->OnUpdate(0.0f);
    }

    Rectangle::~Rectangle()
    {}
    
    void Rectangle::OnUpdate(float deltaTime)
    {
        VertexBuffer vb(m_Points, sizeof(m_Points[0])*4*2);
        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 2);
        va.AddBuffer(vb, layout);
    }

    void Rectangle::OnRender()
    {
        glm::vec3 translation(m_Translation[0], m_Translation[1], 0.f);
        glm::vec3 scaling(m_Scale, m_Scale, 1.f);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        model = glm::scale(model, scaling);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 projection = glm::ortho(0.0f, renderer.Width(), 0.0f, renderer.Height(), -1.0f, 1.0f);
        glm::mat4 mvp = projection * view * model;

        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
    }

    void Rectangle::OnImguiRender()
    {
        ImGui::SliderFloat2("Translation", m_Translation, 0.f, 960.f); //not feeling like changing this slider yet
        if (ImGui::Button("Reset translation")){
            float newTranslation[] = {100.f, 100.f};
            std::copy(std::begin(newTranslation), std::end(newTranslation), std::begin(m_Translation));
        }
        ImGui::Separator();
        ImGui::SliderFloat("Scale", &m_Scale, 0.f, 100.f);
        if (ImGui::Button("Reset scale")){
            m_Scale = 10.f;
        }
        ImGui::Separator();
        ImGui::SliderFloat2("Point 1", &m_Points[0], -10.f, 10.f);
        ImGui::SliderFloat2("Point 2", &m_Points[2], -10.f, 10.f);
        ImGui::SliderFloat2("Point 3", &m_Points[4], -10.f, 10.f);
        ImGui::SliderFloat2("Point 4", &m_Points[6], -10.f, 10.f);
        if (ImGui::Button("Reset points")){
            float newPoints[] = {-1.f, 1.f, 1.f, 1.f, 1.f, -1.f, -1.f, -1.f};
            std::copy(std::begin(newPoints), std::end(newPoints), std::begin(m_Points));
        }
    }

}