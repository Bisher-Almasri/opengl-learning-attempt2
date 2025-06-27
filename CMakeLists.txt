cmake_minimum_required(VERSION 3.10)

project(AtemptTwo VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(SOURCES
    src/main.cpp
    include/glad/glad.c
        src/Shader.cpp
        src/Shader.hpp
        include/stb/stb_image.h
        include/stb/stb_image.c
)

add_executable(AtemptTwo ${SOURCES})

include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(AtemptTwo ${GLFW_LIBRARIES})
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(AtemptTwo ${OPENGL_gl_LIBRARY} dl)
