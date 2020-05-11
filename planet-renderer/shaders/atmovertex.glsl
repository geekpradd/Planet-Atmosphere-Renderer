#version 330 core
layout (location = 0) in vec3 screenCoord;
out vec2 fragCoord;

void main(){
    gl_Position = vec4(screenCoord, 1.0);
    fragCoord = screenCoord.xy;
}