#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "include/utility.hpp"
#include "include/shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // +Z
glm::vec3 upperVec = glm::vec3(0.0f, 1.0f, 0.0f); //+Y
glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 rightVec = glm::normalize(glm::cross(lookAt, upperVec)); // +X

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

    glViewport(0, 0, 800, 600);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(w, cursor_callback);
    glfwSetFramebufferSizeCallback(w, reshape_viewport);
    glfwSetScrollCallback(w, scroll_callback);
    glfwSetKeyCallback(w, key_callback);

    Shader *shdr = new Shader("shaders/lightingvertexshader.glsl", "shaders/lightingfragment.glsl");
    Shader *sourceshdr = new Shader("shaders/lightingvertexshader.glsl", "shaders/sourcefragment.glsl");

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_DEPTH_TEST);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/chess.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    GLuint VBO; // vertex buffer object. stores on gpu buffer
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    GLuint lightVBO; glGenBuffers(1, &lightVBO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // follow model viewing pipeline
    
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);

    while (!glfwWindowShouldClose(w)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        deltaTime = glfwGetTime() - curTime;
        curTime = glfwGetTime(); 
        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f/600.0f, 0.1f, 100.0f);
        
        // for camera we will exclusively modify only the view matrix
        lookAt.y = sin(glm::radians(pitch));
        lookAt.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
        lookAt.z = cos(glm::radians(pitch))*sin(glm::radians(yaw)); 
        rightVec = glm::normalize(glm::cross(lookAt, upperVec)); // +X

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos+lookAt, upperVec);
       
    
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model,  glm::radians(20.0f), glm::vec3(glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))));
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        
        shdr->use();
        shdr->setMatrix4f("model", model);
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
        shdr->setMatrix4f("normalMatrix", normalMatrix);
        shdr->setVec3f("viewLoc", cameraPos);
        shdr->setMatrix4f("view", view);
        shdr->setMatrix4f("projection", projection);
        shdr->setVec3f("lightColor", lightColor);
        shdr->setVec3f("objectColor", objectColor);
        shdr->setVec3f("sourcePos", lightPos);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        sourceshdr->use();
        glBindVertexArray(lightVAO);   
        model = glm::translate(glm::mat4(1.0f), lightPos);
        model = glm::rotate(model,  glm::radians(20.0f), glm::vec3(glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))));
        sourceshdr->setMatrix4f("model", model);
        sourceshdr->setMatrix4f("view", view);
        sourceshdr->setMatrix4f("projection", projection);
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        
        glfwSwapBuffers(w);
        glfwPollEvents();
    } // 
    glfwTerminate();
    return 0;

}