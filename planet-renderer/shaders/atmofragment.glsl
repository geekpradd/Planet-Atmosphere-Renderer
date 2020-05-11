#version 330 core
in vec2 fragCoord;
out vec4 output;

// all in view space
uniform float tanpi8;
uniform vec3 earth;
uniform float PI;
uniform vec3 sun_dir; // normalized
uniform float eRadius;
uniform float aRadius;
uniform float g; // phase retardation factor
uniform float ref;
uniform float kMIe;
uniform vec3 kRlh;
uniform float lightIntensity;


float scale(float fCos)	
{	
	float x = 1.0 - fCos; 	
	return ref * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));	
}

void main(){
    vec3 ray = vec3(4.0/3.0*fragCoord.x*tanpi8, fragCoord.y*tanpi8, -1.0);

    ray = normalize(ray);
    float height_cam = length(earth);
    vec3 start = vec3(0.0f, 0.0f, 0.0f);

    float proj = dot(ray, earth);
    if (proj <= 0)
        discard;
    float d = sqrt(length(earth)*length(earth) - proj*proj);
    if (d >= aRadius)
        discard;
    float t_hc = sqrt(aRadius*aRadius - d*d);

    if (height_cam > aRadius){
         float t_0 = proj - t_hc;
         start = ray*t_0;
    }

    vec3 end;
    if (d >= eRadius){
        float t_1 = proj + t_hc;
        end = ray*t_1;
    }
    else {
        
        float t_e = sqrt(eRadius*eRadius - d*d);
        end = ray*(proj - t_e);
    }
    float nSteps = 9;
    float len = length(end-start)/nSteps;

    // phase values
    vec3 totSc = vec3(0.0f, 0.0f, 0.0f);
    float mu = dot(ray, sun_dir);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    float net_op = 0.0f;

    float fScaleOverScaleDepth = 1/(ref*(aRadius - eRadius));
    for (int i=1; i<=nSteps; ++i){
        vec3 sample = start + i*len*ray;
        float height = length(sample - earth);
        float opDep = exp(fScaleOverScaleDepth * (eRadius - height));	
        
        net_op += opDep*len;
        float light_angle = dot(sun_dir, sample)/(length(sun_dir)*length(sample));
        float cam_angle = dot(sample, earth-sample)/(length(sample)*length(earth-sample));
    
        float outDepth = (opDep*(scale(light_angle) - scale(cam_angle)));
        vec3 mie = vec3(kMIe, kMIe, kMIe);
        vec3 attenuate = exp(-((outDepth + net_op)*(kRlh + mie)));

        totSc += attenuate*len*opDep;
    }

    // output = vec4(lightIntensity * (pRlh * kRlh * totSc + pMie * kMIe * totSc), 1.0f); 
    output = vec4(1.0, 1.0,, 1.0);
    // output = vec4(1.0f, 1.0f, 1.0f, 0.8f);
}