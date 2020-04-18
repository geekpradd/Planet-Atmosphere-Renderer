#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "include/utility.hpp"
#include "include/shader.hpp"

void reshape_viewport(GLFWwindow *w, int width, int height){
    glViewport(0, 0, width, height);
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.5f,
     -0.5f, 0.5f, 0.0f, 0.25f, 0.25f, 0.25f
}; 

GLuint indices[] = {
    0, 3, 1,
    3, 1, 2
};

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);

    glfwMakeContextCurrent(w);
    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(w, reshape_viewport);

    Shader *shdr = new Shader("shaders/vertexshader.glsl", "shaders/fragment.glsl");
    
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    GLuint VBO; // vertex buffer object. stores on gpu buffer
    glGenBuffers(1, &VBO);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(w)){
        shdr->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(w);
        glfwPollEvents();
        glBindVertexArray(0);
    }

    glfwTerminate();
    return 0;

}