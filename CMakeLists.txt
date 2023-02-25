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
    src/main.cpp
    src/glStuff/Renderer.cpp
    src/glStuff/IndexBuffer.cpp
    src/glStuff/VertexBuffer.cpp
    src/glStuff/VertexArray.cpp
    src/glStuff/Shader.cpp
    src/glStuff/Texture.cpp
    #now the IMGUI since they dont have their own CMake
    submodules/imgui_docking/imgui.cpp
    submodules/imgui_docking/imgui_draw.cpp
    submodules/imgui_docking/imgui_demo.cpp # this one isnt necesarry
    submodules/imgui_docking/imgui_tables.cpp
    submodules/imgui_docking/imgui_widgets.cpp
    submodules/imgui_docking/backends/imgui_impl_glfw.cpp
    submodules/imgui_docking/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "submodules/glfw/include"
    "submodules/glew-cmake/include"
    "submodules/glm"
    "submodules/soil2/src"
    "submodules/imgui_docking"
    "submodules/imgui_docking/backends"
    "src"
    "src/glStuff"
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} "glfw;libglew_static;glm;soil2")
