#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "include/utility.hpp"
#include "include/shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "include/cube.hpp"
#include "include/sphere.hpp"
#include "include/skybox.hpp"
#include "include/camera.hpp"
#include "include/screenfiller.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
// Press X to switch DEBUG
// Press B to change from Blinn Phong to Phong
// Press L to switch between light types
bool DEBUG=false;
int blinnPhong = 0;
int globalTextureCount = 0;
int lightType = 1; // 0 = point light, 1 = directional light, 2 = spotlight
float decay_control = 0.01f;
float offset = 0.2f;
glm::vec3 initialCameraPos = glm::vec3(0.0f, 0.0f, 15.0f); // +Z
glm::vec3 initialUpperVec = glm::vec3(0.0f, 1.0f, 0.0f); //+

float decay_rate(glm::vec3 current){
    float length = glm::length(current);
    return (1 - exp(-(decay_control*length+offset)));
}


GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

// this camera always points in -z initially
Camera camera(initialCameraPos, initialUpperVec);
void reshape_viewport(GLFWwindow *w, int width, int height);
void cursor_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* w, double x, double y);
void processInput(GLFWwindow *window);
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLuint load_texture(char const* path);
GLuint load_cube_texture(std::vector< std::string > paths, std::string base);
std::vector<std::string> skybox = {"right.png", "left.png", "top.png", "bottom.png", "front.png", "back.png"};
std::string skybox_base_path = "textures/skybox4/";

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(800, 600, "Interactive Camera Model", NULL, NULL);
    glfwMakeContextCurrent(w);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(w, cursor_callback);
    glfwSetFramebufferSizeCallback(w, reshape_viewport);
    glfwSetScrollCallback(w, scroll_callback);
    // glfwSetKeyCallback(w, key_callback);
    GLuint skyTexture = load_cube_texture(skybox, skybox_base_path);

    
    GLuint ambient = load_texture("textures/earthlights1k.jpg");
    GLuint diffuse = load_texture("textures/Earth.png");
    GLuint specular = load_texture("textures/earthspec1k.jpg");
    Shader *skyShdr = new Shader("shaders/skyboxvertex.glsl", "shaders/skyboxfragment.glsl");
    Shader *sourceshdr = new Shader("shaders/spherevertexshader.glsl", "shaders/sourcefragment.glsl");
    Shader *shdr = new Shader("shaders/spherevertexshader.glsl", "shaders/lightingfragment.glsl");
    Shader *atmo = new Shader("shaders/scatter_vert.glsl", "shaders/scatter_frag.glsl");

    SkyBox skybox;
    Sphere sph(5.0f);
    // Sphere sourcesph(1.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 lightPos(0.0f, 0.0f, 17.0f);
    glm::vec3 lightDirection(1.0f, 0.0f, 0.0f);
    ScreenFiller sf;
    
    
    while (!glfwWindowShouldClose(w)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        
        glm::vec3 current = camera.cameraPos;
        float decay = decay_rate(current);
        camera.updateTimeCounter(decay);
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), 800.0f/600.0f, 0.1f, 100.0f);

        //for camera we will exclusively modify only the view matrix
        camera.updateLookAt();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec4 earth = view*(model*glm::vec4(0.0, 0.0, 0.0, 1.0));
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(w);


        shdr->use();
        shdr->setMatrix4f("view", view);
        shdr->setMatrix4f("model", model);
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
        shdr->setMatrix4f("normalMatrix", normalMatrix);
        shdr->setVec3f("viewLoc", camera.cameraPos);
        shdr->setMatrix4f("view", view);
        shdr->setMatrix4f("projection", projection);
        shdr->setVec3f("light.position", lightPos);
        shdr->setVec3f("light.direction", lightDirection);
        shdr->setInt("light.type", lightType);
        shdr->setFloat("light.constant",  1.0f);
        shdr->setFloat("light.linear",    0.04f);
        shdr->setFloat("light.quadratic", 0.0003f);
         
        shdr->setVec3f("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
        shdr->setVec3f("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        shdr->setVec3f("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        shdr->setInt("material.ambient", 0);
        shdr->setInt("material.diffuse", 1);
        shdr->setInt("material.specular", 2);
        shdr->setFloat("material.shininess", 30.0f);

        glBindTexture(GL_TEXTURE0, ambient);
        glBindTexture(GL_TEXTURE1, diffuse);
        glBindTexture(GL_TEXTURE2, specular);
        glBindVertexArray(sph.VAO);
        glDrawArrays(GL_TRIANGLES, 0, sph.vertex_count);

        // sourceshdr->use();
        // model = glm::translate(glm::mat4(1.0f), lightPos);
        // sourceshdr->setMatrix4f("model", model);
        // sourceshdr->setMatrix4f("view", view);
        // sourceshdr->setMatrix4f("projection", projection);
        // sourceshdr->setVec3f("light", lightColor);
        // glBindVertexArray(sourcesph.VAO);
        
        // glDrawArrays(GL_TRIANGLES, 0, sourcesph.vertex_count);  

        glm::mat4 skyMapView = glm::mat4(glm::mat3(view));  
        glDepthFunc(GL_LEQUAL);
        skyShdr->use();
        skyShdr->setMatrix4f("view", skyMapView);
        skyShdr->setMatrix4f("projection", projection);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
        glBindVertexArray(skybox.VAO);
        glDrawArrays(GL_TRIANGLES, 0, skybox.vertex_count);
        glDepthFunc(GL_LESS);
        atmo->use();
        atmo->setFloat("tangent", tan(glm::radians(camera.fov)/2));
        atmo->setVec3f("earth_center", glm::vec3(earth));

        glm::vec3 lDir = glm::mat3(view)*lightDirection;
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_SRC_ALPHA);
        atmo->setVec3f("light_direction", lDir);
        atmo->setFloat("earth_radius", 5.0f);
        atmo->setFloat("atm_radius", 5.0f*(1.025f));
        
        glBindVertexArray(sf.VAO);
        glDrawArrays(GL_TRIANGLES, 0, sf.vertex_count);

        glDisable(GL_BLEND);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } // 
    glfwTerminate();
    return 0;
}
void reshape_viewport(GLFWwindow *w, int width, int height){
    glViewport(0, 0, width, height);
}
void cursor_callback(GLFWwindow* window, double x, double y){
    float delta_x = x - camera.x_glob;
    float delta_y = camera.y_glob - y;
    camera.x_glob = x;
    camera.y_glob = y;
    camera.modify_pitch_yaw(delta_x, delta_y);
}
void scroll_callback(GLFWwindow* w, double x, double y){
    camera.modify_fov(y);
}

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GL_TRUE);
//     else if (key==GLFW_KEY_X && action==GLFW_PRESS)
//         DEBUG = !DEBUG;
//     else if (key==GLFW_KEY_L && action==GLFW_PRESS)
//         lightType = (lightType+1)%2;
//     else if (key==GLFW_KEY_B && action==GLFW_PRESS)
//         blinnPhong = (blinnPhong+1)%2;
//     else 
//         camera.update_camera_position(key, action);
    
//     if (key != GLFW_KEY_ESCAPE && DEBUG){
//         std::cout << "Current Position is " << camera.cameraPos.x << ", " << camera.cameraPos.y << ", " << camera.cameraPos.z << std::endl;
//     }
    
// }

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.KeyboardInput(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.KeyboardInput(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.KeyboardInput(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.KeyboardInput(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.KeyboardInput(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.KeyboardInput(UP, deltaTime);
}

GLuint load_texture(char const * path)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glActiveTexture(GL_TEXTURE0 + globalTextureCount);
        globalTextureCount++;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

GLuint load_cube_texture(std::vector< std::string > paths, std::string base=""){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    int w, h, nrChannels;
    for (int i=0; i<paths.size(); ++i){
        unsigned char* data = stbi_load((base+paths[i]).c_str(), &w, &h, &nrChannels, 0);
        if (data){
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else 
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cerr << "FAILED TO LOAD CUBE TEXTURE AT LOCATION " << paths[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}