#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {

public:
    glm::vec3 cameraPos; // +Z
    glm::vec3 upperVec; //+Y
    glm::vec3 rightVec; // +X
    glm::vec3 lookAt;
    float yaw = -90.0f; // yaw is measured wrt x axis in xz plane (rotation axis is y)
    float pitch = 0.0f; // pitch is measured wrt z axis in yz plane (rotation axis is x)
    float curTime = glfwGetTime();
    float deltaTime = 0.0f;
    float fov = 45.0f;
    float x_glob, y_glob, speed;

    Camera(glm::vec3 initialPosition, glm::vec3 upperDirection, glm::vec3 lookDir, float width=800, float height=600){
        cameraPos = initialPosition;
        upperVec = upperDirection;
        lookAt = lookDir;

        rightVec = glm::normalize(glm::cross(upperVec, cameraPos)); // +X
        x_glob = width/2; y_glob = height/2;

    }
    void modify_pitch_yaw(float delta_x, float delta_y){
        float senstivity = 0.05f;
        pitch += senstivity*delta_y;
        yaw += senstivity*delta_x;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    
    void modify_fov(double y_offset){
        if (fov > 1.0f && fov <= 45.0f){
            fov -= y_offset;
        }
        else if (fov<=1.0f){
            fov= 1.0f;
        }
        else if (fov > 45.0f){
            fov = 45.0f;
        }
    }
    
    void updateTimeCounter(){
        deltaTime = glfwGetTime() - curTime;
        curTime = glfwGetTime();
        speed = 5000.0f*deltaTime; 
    }
    void updateLookAt(){
        lookAt.y = sin(glm::radians(pitch));
        lookAt.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
        lookAt.z = cos(glm::radians(pitch))*sin(glm::radians(yaw)); 
        rightVec = glm::normalize(glm::cross(lookAt, upperVec));
    }

    glm::mat4 getViewMatrix(){
        return glm::lookAt(cameraPos, cameraPos+lookAt, upperVec);
    }
    
    void update_camera_position(int key, int action){
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            cameraPos = (cameraPos - speed*rightVec);
        else if (key == GLFW_KEY_D && action == GLFW_PRESS)
            cameraPos = (cameraPos + speed*rightVec);
        else if (key == GLFW_KEY_W && action == GLFW_PRESS)
            cameraPos = (cameraPos + speed*lookAt);
        else if (key == GLFW_KEY_S && action == GLFW_PRESS)
            cameraPos = (cameraPos - speed*lookAt);
    }
};