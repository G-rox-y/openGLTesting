#pragma once

#include <vector>
#include <functional>
#include <string>

namespace test
{

    class Test
    {
    public:
        Test(){}
        virtual ~Test(){}

        virtual void OnUpdate(float deltaTime){}
        virtual void OnRender(){}
        virtual void OnRender(int display_w, int display_h){}
        virtual void OnImguiRender(){}
    };

    class TestMenu : public Test
    {
    private:
        Test*& m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    public:
        TestMenu(Test*& current);
        ~TestMenu(){}

        void OnImguiRender() override;

        template<typename T, typename... Args>
        void RegisterTest(const std::string& name, Args&&... args){
            m_Tests.push_back(std::make_pair(name, [&](){ 
                return new T(std::forward<Args>(args)...); 
            } ));
        }
    };

}