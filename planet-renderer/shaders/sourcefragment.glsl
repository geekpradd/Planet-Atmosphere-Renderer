#version 330 core
out vec4 output;

uniform vec3 light;
void main(){
    output = vec4(light, 1.0);
}