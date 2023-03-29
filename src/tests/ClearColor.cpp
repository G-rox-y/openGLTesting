#include "ClearColor.hpp"

#include <GL/glew.h>
#include "imgui.h"

namespace test
{
    ClearColor::ClearColor()
    : m_Color{0.6f, 0.3f, 0.8f, 1.0f}
    {}

    ClearColor::~ClearColor()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void ClearColor::OnUpdate(float deltaTime)
    {}

    void ClearColor::OnImguiRender()
    {
        ImGui::ColorEdit4("Clear color", m_Color);
    }

    void ClearColor::OnRender()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
    }

}