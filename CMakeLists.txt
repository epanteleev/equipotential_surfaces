cmake_minimum_required(VERSION 2.8)

project(Potential)
add_executable(${PROJECT_NAME} main.cpp glad/glad.c glad/glad.h)
target_link_libraries(${PROJECT_NAME} -lglut -lGL -lGLEW -lGLU -LGLM -lglfw -ldl)
