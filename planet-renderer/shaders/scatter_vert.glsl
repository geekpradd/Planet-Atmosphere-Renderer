#version 330 core

in vec3 position;

out vec2 Coords;

void main(){
	gl_Position = vec4(position,1.0);
	Coords = position.xy;
}
