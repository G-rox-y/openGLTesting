#include "Texture.hpp"

#include <GL/glew.h>
#include <SOIL2/SOIL2.h>
#include <iostream>

Texture::Texture(const std::string& filepath)
 : m_RendererID(0), m_Filepath(filepath), m_LocalBuffer(nullptr) ,m_Width(0), m_Height(0), m_BPP(0)
{
    m_LocalBuffer = SOIL_load_image(filepath.c_str(), &m_Width, &m_Height, NULL, SOIL_LOAD_RGBA);

    glGenTextures(1, &m_RendererID);
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

    this->Unbind();
    if (m_LocalBuffer) SOIL_free_image_data(m_LocalBuffer);
    else std::cout << "Failed to load img" << std::endl;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot/*=0*/) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}