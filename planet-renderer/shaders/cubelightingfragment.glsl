#version 330 core
out vec4 output;
in vec3 FragPos;
in vec3 normal;
in vec2 texCoords;
in mat3 TBN;

struct Material {
    sampler2D DIFFUSE;
    vec3 specular;
    float shininess;
    sampler2D NORMAL;
    sampler2D AMBIENT;
    sampler2D ROUGHNESS;
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 specular;
    vec3 diffuse;
    float constant;
    float linear;
    float quadratic;
    int type;
    float cutOff;
    float outerCutOff;
};

uniform vec3 globalAmbient;
uniform Material material;
uniform Light light;
uniform mat4 normalMatrix;
uniform vec3 viewLoc;
uniform int blinnPhong;

void main(){
    int cutOffState = 0;
    float clampIntensity;
    vec3 base = texture(material.DIFFUSE, texCoords).rgb;
    vec3 normal_n = texture(material.NORMAL, texCoords).rgb;
    normal_n = (normal_n*2.0 - 1.0);
    normal_n = normalize(TBN*normal_n);

    vec3 ambient = globalAmbient * base * texture(material.AMBIENT, texCoords).r;

    vec3 lightDir;
    if (light.type == 0) // point light
        lightDir = normalize(light.position - FragPos);
    else if (light.type == 1){
        lightDir = -light.direction;
    }
    else{
        lightDir = normalize(light.position - FragPos);
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon   = light.cutOff - light.outerCutOff;
        clampIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
        if (theta < light.cutOff)
            cutOffState = 1;
    }


    vec3 reflectDir = reflect(-lightDir, normal_n);
    vec3 viewDir = normalize(viewLoc - FragPos);
    vec3 halfway = normalize(lightDir + viewDir);
    float spec;
    if (blinnPhong != 0)
        spec = pow(max(dot(normal_n, halfway), 0.0), material.shininess*(texture(material.ROUGHNESS, texCoords).r));
    else
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*(texture(material.ROUGHNESS, texCoords).r));
    vec3 specular =  spec * material.specular * light.specular;
    float diffuseStrength = max(dot(lightDir, normal_n), 0.0);
    vec3 diffuse = diffuseStrength * base * light.diffuse;

    if (light.type == 0){
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
                        light.quadratic * (distance * distance));  
        specular *= attenuation;
        diffuse *= attenuation;
    }

    vec3 Color =  ambient + diffuse + specular;
    if (cutOffState==1){
        Color = Color - diffuse - specular;
        diffuse *= clampIntensity; specular *= clampIntensity;
        Color = Color + diffuse + specular;
    }

    output = vec4(Color, 1.0f);   
}   