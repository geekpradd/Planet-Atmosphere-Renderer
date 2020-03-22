#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utility.hpp"

void reshape_viewport(GLFWwindow *w, int width, int height){
    glViewport(0, 0, width, height);
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
}; 

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // begin shader pipeline setup
    std::string vertexShader = readFile("vertexshader.glsl");
    const char* vertShad = vertexShader.c_str();
    std::string fragmentShader = readFile("fragment.glsl");
    const char* fragShad = fragmentShader.c_str();
    
    GLFWwindow* w = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);

    glfwMakeContextCurrent(w);
    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(w, reshape_viewport);

    GLuint vShader;
    vShader = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vShader, 1, &vertShad, NULL);
    glCompileShader(vShader);

    GLuint fShader;
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragShad, NULL);
    glCompileShader(fShader);

    GLuint shader;
    shader = glCreateProgram();
    glAttachShader(shader, fShader);
    glAttachShader(shader, vShader);
    glLinkProgram(shader);
    

    glDeleteShader(fShader); glDeleteShader(vShader);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    GLuint VBO; // vertex buffer object. stores on gpu buffer
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    

    while (!glfwWindowShouldClose(w)){
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(w);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}