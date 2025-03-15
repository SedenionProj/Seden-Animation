#version 460

#include "assets/shaders/hash.glsl"

layout(std430, binding = 0) buffer _colBuff
{
   vec4 colBuff[]; 
};

out vec4 FragColor;
in vec2 iFragPos;
uniform float test;

#define MAX_ITER 1000


float mandelbrot(vec2 pos){
    vec2 z = vec2(0);
    float i;
    for (i = 0.; i<float(MAX_ITER); i++){
        z = vec2(z.x*z.x - z.y*z.y, z.x*z.y*2.)+pos;
        if(dot(z,z)>2000.) break;
    }
    
    i = i - log2(log2(dot(z,z))) + 4.0;
    
    return i;
}

vec3 coloring(float i){
    return 0.5*cos(i*.2 + vec3(1,0,0))+0.5;
}

void main(){
    int pixelIndex = int(gl_FragCoord.x+1280*gl_FragCoord.y);
    int frame = int(colBuff[pixelIndex].w);
    g_seed = float(base_hash(floatBitsToUint(gl_FragCoord.xy)))/float(0xffffffffU) + sqrt(float(frame));

    vec3 color = vec3(0,0,0);
    color = colBuff[pixelIndex].xyz;

    colBuff[pixelIndex] += vec4(coloring(mandelbrot(iFragPos)),1);

    FragColor = vec4(coloring(mandelbrot(iFragPos)),1);
}