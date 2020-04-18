#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "include/utility.hpp"

void resize(GLFWwindow *w, int width, int height){
    glViewport(0, 0, width, height);
}

void processinput(GLFWwindow *w){
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(w, true);
    }
    if (glfwGetKey(w, GLFW_KEY_I) == GLFW_PRESS){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);

    glfwMakeContextCurrent(win);

    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(win, resize);

    while(!glfwWindowShouldClose(win)){
        processinput(win);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}