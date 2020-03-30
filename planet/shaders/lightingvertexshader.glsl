#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalVec;

out vec3 normal;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1);
    normal = normalVec;
    FragPos = vec3(model * vec4(pos, 1.0f));
}