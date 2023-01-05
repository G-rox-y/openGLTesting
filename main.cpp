#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//----------------------------------------------------------
struct ShaderSource{
    std::string VertexSource;
    std::string FragmentSource;
};

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
    float coords[8] = {
        -0.5f, -0.5f, 
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer; //openGL stores the buffer index in this int
    glGenBuffers(1, &buffer); //generate the buffer and save its index in buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //here GL sets the buffer as the data that further operations will be done on
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords[0])*4*2, coords, GL_STATIC_DRAW); //pass data to buffer
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(coords[0])*2, (const void*)0); //explaining the data in the buffer
    glEnableVertexAttribArray(0); // it has to be enabled manualy for some reason


    unsigned int ibo; //index buffer object
    glGenBuffers(1, &ibo); //generate the ibo address
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //do shit here
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*3*2, indices, GL_STATIC_DRAW); //pass data to the shit


    ShaderSource source = ParseShader("resources/shaders/shader.glsl");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}