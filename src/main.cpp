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

#include "glStuff/Texture.hpp"
#include "glStuff/Renderer.hpp"

//----------------------------------------------------------

const float PI = 3.14159265;

int main(void){
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW version: " << GLFW_VERSION_MAJOR <<'.'<< GLFW_VERSION_MINOR <<'.'<< GLFW_VERSION_REVISION << std::endl;
    std::cout << "GLEW version: " << GLEW_VERSION_MAJOR <<'.'<< GLEW_VERSION_MINOR <<'.'<< GLEW_VERSION_MICRO << std::endl;
    std::cout << "SOIL2 version: " << SOIL_MAJOR_VERSION <<'.'<< SOIL_MINOR_VERSION <<'.'<< SOIL_PATCH_LEVEL << std::endl;
    std::cout << "GLM version: " << GLM_VERSION_MAJOR <<'.'<< GLM_VERSION_MINOR <<'.'<< GLM_VERSION_REVISION << std::endl;
    std::cout << "IMGUI version: " << IMGUI_VERSION << std::endl;

    //create window object
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;


    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    //add modern openGL
    const GLenum err = glewInit();
    if (err != GLEW_OK){
        std::cout << "GLEW init error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION(); // check that version is compatible with what u using it for
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
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


    //setup done, now onto the actual program

    //triangle data
    float coords[] = {
        200.0f, 200.0f, 0.0f, 0.0f,
        200.0f, 100.0f, 0.0f, 1.0f,
        100.0f, 100.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 1.0f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    //enable blending and set it
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //create vertex array
    VertexArray va;

    //create vertex buffer from data and add it to the vertex array
    VertexBuffer vb(coords, sizeof(coords[0])*4*4);
    VertexBufferLayout layout;
    layout.Push(GL_FLOAT, 2);
    layout.Push(GL_FLOAT, 2);
    va.AddBuffer(vb, layout);

    //create index buffer from data
    IndexBuffer ib(indices, 6);

    //create renderer
    Renderer renderer;

    //create shader
    Shader shader("resources/shaders/shader.glsl");

    //add texture to shader
    Texture texture("resources/images/srce.png");
    texture.Bind(0); // bind texture to slot 0
    shader.SetUniform1i("u_Texture", 0); // create uniform that says what slot the texture is in

    //add model-view-projection matrix to shader
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    // ^ create orthographic projection matrix that creates a 2D image from everything within the
    // X coordinates from -2 to 2, Y from -1.5 to 1.5, and Z from -1 to 1
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); // translate view matrix (I) 100 to left

    glm::vec3 translation(200, 200, 0);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window)){
        //clear back buffer
        renderer.Clear();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation); // translate model matrix (I)
        glm::mat4 mvp = proj * view * model; // create MVP matrix
        
        shader.SetUniformMat4f("u_MVP", mvp);
        //set the color of the fragment shader (uniform) you could animate this if ya wanted
        shader.SetUniform4f("u_Color", 1.f, 1.f, 0.f, 1.f);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // show imgui window(s)
        // ImGui::ShowDemoWindow();

        {
            ImGui::SliderFloat3("Translation", &translation.x, 0.f, 960.f);
        }

        //render imgui window()
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        // For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        

        { //handle window resizing
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            renderer.Resize(display_w, display_h);
        }
        //draw the va data, with the ib layout using the shader
        renderer.Draw(va, ib, shader);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}