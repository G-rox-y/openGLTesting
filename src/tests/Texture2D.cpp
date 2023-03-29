#include "Texture2D.hpp"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

namespace test
{
    Texture2D::Texture2D(Renderer& renderer, int& display_w, int& display_h)
    : m_translationA(200, 200, 0), m_translationB(400, 200, 0), shader("resources/shaders/Texture2D.glsl"),
      texture("resources/images/Texture2D_texture.png"), renderer(renderer)
    {
        //triangle data
        float coords[] = {
            50.0f, 50.0f, 0.0f, 0.0f, //rectangle x, rectangle y, texture x, texture y
            50.0f, -50.0f, 0.0f, 1.0f,
            -50.0f, -50.0f, 1.0f, 1.0f,
            -50.0f, 50.0f, 1.0f, 0.0f,
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        //enable blending and set it
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //create vertex buffer from data and add it to the vertex array
        VertexBuffer vb(coords, sizeof(coords[0])*4*4);
        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 2);
        layout.Push(GL_FLOAT, 2);
        va.AddBuffer(vb, layout);

        //insert data to index buffer
        ib.AddData(indices, 6);

        //add texture to shader
        texture.Bind(0); // bind texture to slot 0
        shader.SetUniform1i("u_Texture", 0); // create uniform that says what slot the texture is in

        m_proj = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, -1.0f, 1.0f);
        // ^ create orthographic projection matrix that creates a 2D image from everything within the
        // X coordinates from -2 to 2, Y from -1.5 to 1.5, and Z from -1 to 1
        // this is not on a constructor so i can make comments more visible, but it can go there
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // translate view matrix (I) 100 to left
    }

    Texture2D::~Texture2D()
    {
        //return blending back to disabled
        glDisable(GL_BLEND);
        //set bind texture to 0 (redundant but cleanup is cleanup) 
        texture.Unbind();
    }

    void Texture2D::OnUpdate(float deltaTime)
    {}

    void Texture2D::OnRender()
    {
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA); // translate model matrix (I)
            glm::mat4 mvp = m_proj * m_view * model; // create MVP matrix
            shader.SetUniformMat4f("u_MVP", mvp);
            //draw the va data, with the ib layout using the shader
            renderer.Draw(va, ib, shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
            glm::mat4 mvp = m_proj * m_view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }
    }

    void Texture2D::OnImguiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.f, 960.f);
        ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.f, 960.f);
    }

}