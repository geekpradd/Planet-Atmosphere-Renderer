#version 330 core
out vec4 output;

uniform vec3 objectColor;
uniform vec3 lightColor;
void main(){
    float ambientStrength = 0.1;
    vec3 ambientColor = ambientStrength*lightColor;
    output = vec4(objectColor*ambientColor, 1.0);
}