#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utility.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Shader{
    public:
        GLuint id;
        Shader(const char * vertexSource, const char* fragSource);
        void use();
        void setFloat(const char* uniformID, float value);
        void setInt(const char* uniformID, int value);
        void setVec3f(const char* uniformID, glm::vec3 vector);
        void setVec4f(const char* uniformID, glm::vec4 vector);
        void setMatrix4f(const char* uniformID, glm::mat4 matrix);
};

