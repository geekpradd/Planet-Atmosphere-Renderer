#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalVec;
layout (location = 2) in vec2 textCo;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec3 tangent;

out vec3 normal;
out vec2 texCoords;
out vec3 FragPos;
out mat3 TBN;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1);
    normal = normalVec;
    FragPos = vec3(model * vec4(pos, 1.0f));
    texCoords = textCo;
    TBN = mat3(tangent, bitangent, normalVec);
}