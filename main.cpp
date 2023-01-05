#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

//----------------------------------------------------------

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
    float coords[6] = {
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f
    };

    unsigned int buffer; //openGL stores the buffer index in this int
    glGenBuffers(1, &buffer); //generate the buffer and save its index in buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //here GL sets the buffer as the data that further operations will be done on
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords[0])*6, coords, GL_STATIC_DRAW); //pass data to buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(coords[0])*2, (const void*)0); //explaining the data in the buffer
    glEnableVertexAttribArray(0); // it has to be enabled manualy for some reason

std::string vertexShader = 
R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

void main(){
    gl_Position = position;
}
)glsl";

std::string fragmentShader = 
R"glsl(
#version 330 core

layout(location = 0) out vec4 color;

void main(){
    color = vec4(1.0, 0.0, 0.0, 1.0);
}
)glsl";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}