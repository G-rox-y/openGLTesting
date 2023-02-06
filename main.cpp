#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

//----------------------------------------------------------
struct ShaderSource{
    std::string VertexSource;
    std::string FragmentSource;
};

const float PI = 3.14159265;

static ShaderSource ParseShader(const std::string &filepath){
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

static unsigned int CompileShader(unsigned int type, const std::string& source){
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;


    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
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

    unsigned int vao;
    glGenVertexArrays(1, &vao); //create a vertex array object
    glBindVertexArray(vao); //bind the vertex array object to be currently active
    
    unsigned int buffer; //openGL stores the buffer index in this int
    glGenBuffers(1, &buffer); //generate the buffer and save its index in &buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //here GL sets the buffer as the data that further operations will be done on
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords[0])*7*2, coords, GL_STATIC_DRAW); //pass data to buffer
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(coords[0])*2, (const void*)0); //explaining the data in the buffer
    // ^ works as following: location 0 of VAO (currently bound vertex array) will be filled with 2 pairs of floats
    // without normalisation, each pair will have size of two sizeof(coords[0])
    // and gpu should search for them at the offset of 0 from the currently bound GL_ARRAY_BUFFER
    glEnableVertexAttribArray(0); // it has to be enabled manualy for some reason


    unsigned int ibo; //index buffer object
    glGenBuffers(1, &ibo); //generate the ibo address
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //do shit here
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*3*3, indices, GL_STATIC_DRAW); //pass data to the shit


    ShaderSource source = ParseShader("resources/shaders/shader.glsl");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    int location = glGetUniformLocation(shader, "u_color");
    if (location == -1) std::cout << "eh, it doesnt exist" << std::endl;

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);

        //activate shader
        glUseProgram(shader);     
        //set the color of the fragment shader (uniform) you could animate this if ya wanted   
        glUniform4f(location, 1.f, 1.f, 0.f, 1.f);

        //bind vertex array, this binds the buffer since the vertexattribpointer call linked buffer data with the vao
        glBindVertexArray(vao);
        //this binds the ibo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}