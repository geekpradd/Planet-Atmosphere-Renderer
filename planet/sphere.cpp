#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "include/utility.hpp"
#include "include/shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "include/sphere.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f); // +Z
glm::vec3 upperVec = glm::vec3(0.0f, 1.0f, 0.0f); //+Y
glm::vec3 rightVec = glm::normalize(glm::cross(upperVec, cameraPos)); // +X
glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
float yaw = -90.0f; // yaw is measured wrt x axis in xz plane (rotation axis is y)
float pitch = 0.0f; // pitch is measured wrt z axis in yz plane (rotation axis is x)
float curTime = glfwGetTime();
float deltaTime = 0.0f;

void reshape_viewport(GLFWwindow *w, int width, int height){
    glViewport(0, 0, width, height);
}

float x_glob = 400;
float y_glob = 300;
void cursor_callback(GLFWwindow* window, double x, double y){
    float delta_x = x - x_glob;
    float delta_y = y_glob - y;
    x_glob = x;
    y_glob = y;

    float senstivity = 0.05f;
    pitch += senstivity*delta_y;
    yaw += senstivity*delta_x;


    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}
float fov = 45.0f;

void scroll_callback(GLFWwindow* w, double x, double y){
    if (fov > 1.0f && fov <= 45.0f){
        fov -= y;
    }
    else if (fov<=1.0f){
        fov= 1.0f;
    }
    else if (fov > 45.0f){
        fov = 45.0f;
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    float speed = 5000.0f*deltaTime;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
      cameraPos = (cameraPos - speed*rightVec);
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
      cameraPos = (cameraPos + speed*rightVec);
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
      cameraPos = (cameraPos + speed*lookAt);
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
      cameraPos = (cameraPos - speed*lookAt);

    if (key != GLFW_KEY_ESCAPE){
        std::cout << "Current Position is " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
    }
    
    }

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);

    glfwMakeContextCurrent(w);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(w, cursor_callback);
    glfwSetFramebufferSizeCallback(w, reshape_viewport);
    glfwSetScrollCallback(w, scroll_callback);
    glfwSetKeyCallback(w, key_callback);

    Shader *shdr = new Shader("shaders/spherevertexshader.glsl", "shaders/simplefragment.glsl");

    Sphere sph (5.0f);
    
    shdr->use();

    while (!glfwWindowShouldClose(w)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        
        deltaTime = glfwGetTime() - curTime;
        curTime = glfwGetTime(); 
        glBindVertexArray(sph.VAO);
        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f/600.0f, 0.1f, 100.0f);
        shdr->setMatrix4f("projection", projection);

        //for camera we will exclusively modify only the view matrix
        lookAt.y = sin(glm::radians(pitch));
        lookAt.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
        lookAt.z = cos(glm::radians(pitch))*sin(glm::radians(yaw)); 
        rightVec = glm::normalize(glm::cross(lookAt, upperVec)); // +X


        glm::mat4 view = glm::lookAt(cameraPos, cameraPos+lookAt, upperVec);
       
        shdr->setMatrix4f("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        shdr->setMatrix4f("model", model);
          
        glDrawArrays(GL_TRIANGLES, 0, sph.vertex_count);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } // 
    glfwTerminate();
    return 0;
}
