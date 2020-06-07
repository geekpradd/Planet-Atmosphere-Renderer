#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum Cam_mov {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROTLEFT,
    ROTRIGHT
};

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

    Camera(glm::vec3 initialPosition, glm::vec3 upperDirection, float width=800, float height=600){
        cameraPos = initialPosition;
        upperVec = upperDirection;

        rightVec = glm::normalize(glm::cross(upperVec, cameraPos)); // +X
        x_glob = width/2; y_glob = height/2;

    }
    void modify_pitch_yaw(float delta_x, float delta_y){
        float senstivity = 0.05f;
        pitch += senstivity*delta_y;
        yaw += senstivity*delta_x;

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
    
    void updateTimeCounter(float decay = 1.0f){
        deltaTime = glfwGetTime() - curTime;
        curTime = glfwGetTime();
        // std::cout << "decay is " << decay << std::endl;
        speed = 10000.0f*deltaTime*decay; 
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
    
    void KeyboardInput(Cam_mov direction, float deltaTime){
        
        float velocity = speed * deltaTime;
        if(direction == FORWARD)
            cameraPos += lookAt*velocity;
        else if(direction == BACKWARD)
            cameraPos -= lookAt * velocity;
        else if(direction == RIGHT)
            cameraPos += rightVec*velocity;
        else if(direction == LEFT)
            cameraPos -= rightVec*velocity;

    }
};