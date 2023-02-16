cmake_minimum_required(VERSION 3.20)

if(NOT EXISTS "./build")
    file(MAKE_DIRECTORY "build")
endif()

project("OpenGLTutorial")

set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)
set(glew-cmake_BUILD_SHARED OFF)
set(ONLY_LIBS ON)
set(GLFW_BUILD_DOCS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(submodules/glfw)
add_subdirectory(submodules/glew-cmake)
add_subdirectory(submodules/glm)
add_subdirectory(submodules/soil2)

add_executable(${PROJECT_NAME} 
    main.cpp
    Renderer.cpp
    IndexBuffer.cpp
    VertexBuffer.cpp
    VertexArray.cpp
    VertexBufferLayout.cpp
    Shader.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "submodules/glfw/include"
    "submodules/glew-cmake/include"
    "submodules/glm"
    "submodules/soil2/src"
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} "glfw;libglew_static;glm;soil2")
