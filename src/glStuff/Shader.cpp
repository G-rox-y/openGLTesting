#include "Shader.hpp"

#include <GL/glew.h>

#include <sstream>
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& filepath)
: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = this->ParseShader(filepath);
    m_RendererID = this->CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}
void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int v)
{
    this->Bind();
    glUniform1i(this->GetUniformLocation(name), v);
    this->Unbind();
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    this->Bind();
    glUniform4f(this->GetUniformLocation(name), v0, v1, v2, v3);
    this->Unbind();
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    this->Bind();
    glUniformMatrix4fv(this->GetUniformLocation(name), 1, false, &matrix[0][0]);
    this->Unbind();
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) std::cout << "eh, uniform " << name << " doesnt exist" << std::endl;
    m_UniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{
    std::ifstream data(filepath);
    std::string line;
    std::stringstream ss[2];
    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType shader = ShaderType::NONE;

    while(getline(data, line)){
        if(line.find("#shader") != std::string::npos){
            if (line.find("vertex") != std::string::npos)
                shader = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                shader = ShaderType::FRAGMENT;
        }
        else{
            ss[(int)shader] << line << "\n";
        }
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile the shader: ";
        if (type == GL_VERTEX_SHADER) std::cout << "vertex" << std::endl;
        else std::cout << "fragment" << std::endl;
        std::cout << "Error: \n" << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = this->CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = this->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}