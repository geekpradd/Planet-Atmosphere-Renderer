#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utility.hpp"

class Shader{
    public:
        GLuint id;
        Shader(const char * vertexSource, const char* fragSource);
        void use();
        void setFloat(const char* uniformID, float value);
};

