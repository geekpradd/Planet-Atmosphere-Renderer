#version 330 core
out vec4 output;
in vec3 FragPos;
in vec3 normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 sourcePos;
uniform mat4 normalMatrix;
uniform vec3 viewLoc;

void main(){
    float ambientStrength = 0.1;
    vec3 normal_n = mat3(normalMatrix) * normalize(normal);
    vec3 lightDir = normalize(sourcePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal_n);
    vec3 viewDir = normalize(viewLoc - FragPos);

    float specularStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    specularStrength *= spec;

    float diffuseStrength = max(dot(lightDir, normal_n), 0.0f);
    vec3 Color = (ambientStrength + diffuseStrength + spec)*lightColor;
    output = vec4(objectColor*Color, 1.0);
}