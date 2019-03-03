cmake_minimum_required(VERSION 2.8)

project(Potential)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/glfw)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

file(GLOB VENDORS_SOURCES libs/glad/src/glad.c)


include_directories(libs/glad/include/
                    libs/glfw/include/
                    libs/glm/)


add_executable(${PROJECT_NAME} main.cpp ${VENDORS_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
