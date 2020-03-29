#include "shader.hpp"

Shader::Shader(const char * vertexSource, const char* fragSource){
    int success;
    char errorLog[512];

    std::string vert = readFile(vertexSource);
    std::string frag = readFile(fragSource);

    const char* vertShader = vert.c_str();
    const char* fragShader = frag.c_str();

    GLuint vshad;
    vshad = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshad, 1, &vertShader, NULL);
    glCompileShader(vshad);

    glGetShaderiv(vshad, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vshad, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errorLog << std::endl;
    }

    GLuint fshad;
    fshad = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshad, 1, &fragShader, NULL);
    glCompileShader(fshad);

    glGetShaderiv(fshad, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fshad, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << errorLog << std::endl;
    }

    id = glCreateProgram();
    glAttachShader(id, fshad);
    glAttachShader(id, vshad);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success){
        glGetProgramInfoLog(id, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog << std::endl;
    }

    glDeleteShader(vshad);
    glDeleteShader(fshad);
}

void Shader::use(){
    glUseProgram(id);
}

void Shader::setFloat(const char* uniformID, float value){
    glUniform1f(glGetUniformLocation(id, uniformID), value);
}

