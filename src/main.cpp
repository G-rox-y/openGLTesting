#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cmath>

#include "glStuff/Renderer.hpp"

#include "tests/ClearColor.hpp"
#include "tests/Rectangle.hpp"
#include "tests/Texture2D.hpp"

//----------------------------------------------------------

const float PI = 3.14159265;

int main(void)
{
    int display_w=960, display_h=540;

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(display_w, display_h, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //add modern OpenGL
    const GLenum err = glewInit();
    if (err != GLEW_OK){
        std::cout << "GLEW init error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW version: " << GLFW_VERSION_MAJOR <<'.'<< GLFW_VERSION_MINOR <<'.'<< GLFW_VERSION_REVISION << std::endl;
    std::cout << "GLEW version: " << GLEW_VERSION_MAJOR <<'.'<< GLEW_VERSION_MINOR <<'.'<< GLEW_VERSION_MICRO << std::endl;
    std::cout << "SOIL2 version: " << SOIL_MAJOR_VERSION <<'.'<< SOIL_MINOR_VERSION <<'.'<< SOIL_PATCH_LEVEL << std::endl;
    std::cout << "GLM version: " << GLM_VERSION_MAJOR <<'.'<< GLM_VERSION_MINOR <<'.'<< GLM_VERSION_REVISION << std::endl;
    std::cout << "IMGUI version: " << IMGUI_VERSION << std::endl;

    IMGUI_CHECKVERSION(); // check that version is compatible with what u using it for
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); //glsl version

    Renderer renderer(display_w, display_h);

    //open test framework
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    //run test
    testMenu->RegisterTest<test::ClearColor>("Clear Color");
    testMenu->RegisterTest<test::Rectangle>("Rectangle", renderer);
    testMenu->RegisterTest<test::Texture2D>("Texture2D", renderer, display_w, display_h);

    while (!glfwWindowShouldClose(window)){
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest){
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-")){
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImguiRender();
            ImGui::End();
        }

        // ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // Update and Render additional Platform Windows (imgui docking thing)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        
        { //handle window resizing
            glfwGetFramebufferSize(window, &display_w, &display_h);
            renderer.Resize(display_w, display_h);
        }

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    delete currentTest;
    if (currentTest != testMenu)
        delete testMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}