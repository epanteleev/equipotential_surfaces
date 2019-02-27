#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
static const unsigned int SCR_WIDTH = 1000;
static const unsigned int SCR_HEIGHT = 600;
std::vector<float> charges;
static const char *vertexShaderSource = "attribute vec3 aVertexPosition;\
                                    void main(void)\
                                    {\
                                        gl_Position = vec4(aVertexPosition, 1.0);\
                                    }";

static const char *fragmentShaderSource =
        "#version 330 core\n\
        uniform vec3 charges[40];\
        uniform int index;\
        void main(void)\
        {\
            vec3 ch = vec3(0.0);\
            for(int i = 0; i < index; i++)\
            {\
                vec2 r = gl_FragCoord.xy - charges[i].xy;\
                ch += charges[i].z * vec3(0, 0, 1.0 / length(r));\
            }\
            float q = sign(ch.z) * floor(pow(length(ch.z), 0.3) * 25.0) / 25.0;\
            gl_FragColor = vec4(q, 0, -q, 1) + vec4(1 - abs(q));\
            gl_FragColor.rgb -= (step(min(fract(gl_FragCoord.xy / 20.0).x,fract(gl_FragCoord.xy / 20.0).y), 1.0 / 20.0)\
                     + step(min(fract(gl_FragCoord.xy / 100.0).x,fract(gl_FragCoord.xy / 100.0).y), 1.0 / 100.0)) * 0.2;\
        }";


void mouse_button_callback(GLFWwindow* window, int button, int action, int)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        charges.push_back(static_cast<float>(xpos));
        charges.push_back(SCR_HEIGHT - static_cast<unsigned int>(ypos));
        charges.push_back( 1.0f);
    }else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        charges.push_back(static_cast<float>(xpos));
        charges.push_back(SCR_HEIGHT - ypos);
        charges.push_back( -1.0f);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Equipotential surfaces", nullptr, nullptr);
    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback( window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
         1, 1, 0, -1, 1, 0, 1, -1, 0, -1, -1, 0
    };

    unsigned int VBO{}, VAO{};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "aVertexPosition"));
    glVertexAttribPointer(glGetAttribLocation(shaderProgram, "aVertexPosition"), 3, GL_FLOAT, false, 0, 0);

    glClearColor(0.3f,0.2f,0.4f,1);

    glUseProgram(shaderProgram);
    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform3fv(glGetUniformLocation(shaderProgram, "charges"), charges.size() / 3, charges.data());
        glUniform1i(glGetUniformLocation(shaderProgram, "index"), charges.size() / 3);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

