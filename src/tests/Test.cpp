#include "Test.hpp"

#include "imgui.h"

namespace test
{

    TestMenu::TestMenu(Test*& current)
    : m_CurrentTest(current)
    {}

    void TestMenu::OnImguiRender()
    {
        for(auto& test : m_Tests){
            if (ImGui::Button(test.first.c_str()))
                m_CurrentTest = test.second();
        }
    }

}