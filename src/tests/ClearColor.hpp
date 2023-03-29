#pragma once

#include "Test.hpp"

namespace test
{

    class ClearColor : public Test
    {
    public:
        ClearColor();
        ~ClearColor();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImguiRender() override;

    private:
        float m_Color[4];
        
    };

}