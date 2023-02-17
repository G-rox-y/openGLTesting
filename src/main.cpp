#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

//----------------------------------------------------------

const float PI = 3.14159265;

int main(void)
{
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;


    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW version: " << GLFW_VERSION_MAJOR <<'.'<< GLFW_VERSION_MINOR <<'.'<< GLFW_VERSION_REVISION << std::endl;
    std::cout << "GLEW version: " << GLEW_VERSION_MAJOR <<'.'<< GLEW_VERSION_MINOR <<'.'<< GLEW_VERSION_MICRO << std::endl;

    //add modern openGL
    const GLenum err = glewInit();
    if (err != GLEW_OK){
        std::cout << "GLEW init error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }
    
    //triangle data
    float coords[] = {
        0.0f, 1.0f, 
        (float)cos(18.f * PI/180.f), (float)sin(18.f * PI/180.f),
        (float) cos(54 * PI/180.f), (float) -sin(54 * PI/180.f),
        (float) -cos(54 * PI/180.f), (float) -sin(54 * PI/180.f),
        (float) -cos(18 * PI/180.f), (float) sin(18 * PI/180.f),
        (float) cos(54 * PI/180.f) * sin(18 * PI/180.f) / sin(126 * PI/180.f), (float) sin(54 * PI/180.f) * sin(18 * PI/180.f) / sin(126 * PI/180.f),
        (float) cos(18 * PI/180.f) * sin(18 * PI/180.f) / sin(126 * PI/180.f), (float) -sin(18 * PI/180.f) * sin(18 * PI/180.f) / sin(126 * PI/180.f)
    };
    unsigned int indices[] = {
        0, 3, 6,
        2, 4, 5,
        1, 5, 6,
    };

    //create vertex array
    VertexArray va;

    //create vertex buffer from data and add it to the vertex array
    VertexBuffer vb(coords, sizeof(coords[0])*7*2);
    VertexBufferLayout layout;
    layout.Push(GL_FLOAT, 2);
    va.AddBuffer(vb, layout);

    //create index buffer from data
    IndexBuffer ib(indices, 9);

    //create shader and renderer
    Shader shader("resources/shaders/shader.glsl");
    Renderer renderer;

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        //clear back buffer
        renderer.Clear();

        //set the color of the fragment shader (uniform) you could animate this if ya wanted
        shader.SetUniform4f("u_color", 1.f, 1.f, 0.f, 1.f);

        //draw the va data, with the ib layout using the shader
        renderer.Draw(va, ib, shader);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}