#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utility.hpp"

class Shader{
    GLuint id;
    public:
        Shader(const char * vertexSource, const char* fragSource);
        void use();
};

