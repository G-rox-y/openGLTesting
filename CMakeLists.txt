cmake_minimum_required(VERSION 3.20)

if(NOT EXISTS "./build")
    file(MAKE_DIRECTORY "build")
endif()

project("OpenGLTutorial")

set(BUILD_SHARED_LIBS OFF)
set(glew-cmake_BUILD_SHARED OFF)
set(ONLY_LIBS ON)
set(GLFW_BUILD_DOCS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(submodules/glfw)
add_subdirectory(submodules/glew-cmake)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC "submodules/glfw/include"
    PUBLIC "submodules/glew-cmake/include"
    PUBLIC ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} "glfw;libglew_static")
