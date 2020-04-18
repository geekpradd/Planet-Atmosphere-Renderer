#version 330 core
out vec4 output;
in vec3 FragPos;
in vec3 normal;

struct Material {
    vec3 ambient; // this is the light it reflects off given these conditions
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};

uniform Material material;
uniform Light light;
uniform mat4 normalMatrix;
uniform vec3 viewLoc;

void main(){
    vec3 ambient = material.ambient * light.ambient;

    vec3 normal_n = mat3(normalMatrix) * normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal_n);
    vec3 viewDir = normalize(viewLoc - FragPos);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  spec * material.specular * light.specular;
    float diffuseStrength = max(dot(lightDir, normal_n), 0.0);
    vec3 diffuse = diffuseStrength * material.diffuse * light.diffuse;

    vec3 Color = specular + ambient + diffuse;
    output = vec4(Color, 1.0);
}   