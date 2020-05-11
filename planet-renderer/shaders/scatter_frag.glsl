#version 330 core

in vec2 Coords;

out vec4 FragColor;

uniform float earth_radius;
uniform float atm_radius;
uniform vec3 earth_center;
uniform vec3 light_direction;       //light_direction is from sun not towards and normalized
uniform float tangent;

float g = -0.85f;
float H = (atm_radius - earth_radius)/4.0f;
float K_R = 0.166;
float K_M = 0.0025;
vec3 wave_factor = vec3(0.3, 0.7, 1.0);
float PI = radians(180.0f);

float nOutScat = 10;
float nInScat = 10;

float rayleigh(float fcos){
    return 0.75 * (1 + fcos * fcos);
}

float mie(float fcos){
    return 1.5 * ((1 - g * g) * (1 + fcos * fcos))/((2 + g*g)*pow((1 + g*g - 2 * g * fcos),1.5));
}

bool intersect_detect(vec3 start, vec3 direction, vec3 center, float radius){
    float B = 2 * dot(center - start, direction); //sum of roots
    float C = length(start - center)*length(start - center) - radius*radius; //product of roots
    if(B<0 && C>0)
        return false;

    float det = B*B - 4.0*C;
    if(det <= 0)
        return false;

    return true;
}

vec2 intersect_find(vec3 start, vec3 direction, vec3 center, float radius){ //direction must be normalized
    float B = 2 * dot(center - start, direction); //sum of roots
    float C = length(start - center)*length(start - center) - radius*radius; //product of roots
    float det = B*B - 4.0*C;
    vec2 points = vec2(0.5*(B - sqrt(det)), 0.5*(B + sqrt(det)));
    return points;
}

float outscatter(vec3 p1, vec3 p2){   //direction is from p1
    vec3 stepSize = (p2-p1)/nOutScat;
    vec3 samplePoint = p1 + 0.5 * stepSize;

    float scaledStep = length(stepSize)/(atm_radius - earth_radius);

    float sum = 0;
    for(int i =0; i<nOutScat; i++){
        float height = length(samplePoint - earth_center) - earth_radius;
        sum += exp(-height/H);
        samplePoint += stepSize;
    }
    sum = sum * scaledStep;
    float outScat = sum * 4.0 * PI;
    return outScat;
}

vec3 inscatter(vec3 near, vec3 far, vec3 direction){       //near and far are the start and end positions and direction is normalized
    vec3 stepSize = (far - near)/nInScat;
    vec3 samplePoint = near + 0.5 * stepSize;
    float scaledStep = length(stepSize)/(atm_radius - earth_radius);

    vec3 sum = vec3(0.0);

    for(int i =0; i<nInScat; i++){
        vec2 sun_end = intersect_find(samplePoint, -light_direction, earth_center, atm_radius);

        if(!intersect_detect(samplePoint, -light_direction, earth_center, earth_radius)){     //we draw only if light from sun reaches point (not hit earth)
            float height = length(samplePoint - earth_center) - earth_radius;
            float sunOutScat = outscatter(samplePoint, samplePoint - light_direction * sun_end.y);
            float camOutScat = outscatter(samplePoint, near);
            sum += exp(-height/H) * exp(-(sunOutScat + camOutScat) * (K_R * wave_factor + K_M));
        }
        samplePoint += stepSize;
    }

    sum = sum * scaledStep;
    float angle = dot(direction, light_direction);
    vec3 color = sum * (K_R * wave_factor * rayleigh(angle) + K_M * mie(angle)) * 10;
    return color;
}

void main(){
    vec3 direction = vec3(4.0/3.0*Coords.x*tangent, Coords.y*tangent, -1.0);

    direction = normalize(direction);
    // vec3 direction = normalize(vec3(Coords, -1.0/tan(radians(fov/2))));
    if(intersect_detect(vec3(0.0), direction, earth_center, atm_radius)){
        if(intersect_detect(vec3(0.0), direction, earth_center, earth_radius)){ //If lightray hits atmosphere and earth

            //we need to make sure now the values are positive
            vec2 dist_atm = intersect_find(vec3(0.0), direction, earth_center, atm_radius);
            vec2 dist_earth = intersect_find(vec3(0.0), direction, earth_center, earth_radius);
            float near = dist_atm.x;
            float far = dist_earth.x;
            if(near<0)
                near = 0;
            vec3 col = inscatter(direction * near, direction * far, direction);

            FragColor = vec4(col,exp(-outscatter(direction * near, direction * far) * (K_R + K_M)));
            //FragColor = vec4(0.6,0.6,0.0,0.7);
        }
        else{ //if hits at 2 points in atmosphere

            vec2 dist_atm = intersect_find(vec3(0.0), direction, earth_center, atm_radius);
            float near = dist_atm.x;
            float far = dist_atm.y;
            if(near<0)
                near = 0;
            vec3 col = inscatter(direction * near, direction * far, direction);

            FragColor = vec4(col,exp(-outscatter(direction * near, direction * far) * (K_R + K_M)));
            //FragColor = vec4(0.0,0.6,0.6,0.7);
        }
    }
    else{
        discard;
        //FragColor = vec4(1.0,1.0,0.0,0.4);
    }
}
