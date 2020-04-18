#version 330 core
in vec2 texCoord;
out vec4 output;

uniform sampler2D TEXTURE;
void main(){
    output = texture(TEXTURE, texCoord);
}