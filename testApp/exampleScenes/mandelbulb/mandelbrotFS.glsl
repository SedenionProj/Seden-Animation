#version 460

#include "C:/Users/Gerome/source/repos/Seden-Animation/Seden/assets/shaders/hash.glsl"

layout(std430, binding = 0) buffer _colBuff {
   vec4 colBuff[]; 
};

out vec4 FragColor;
in vec2 iFragPos;
uniform vec2 iResolution;

uniform float zoom;
uniform float libre;
uniform float angle;
uniform vec2 pos;
uniform vec3 color;
uniform int iPass;
uniform vec2 iTime;

#define MAX_ITER 1000

mat2 rot(float theta){
    float c = cos(theta);
    float s = sin(theta);
    return mat2(
        vec2( c,-s),
        vec2( s, c)
    );
}

float mandelbrot(dvec2 position) {
    dvec2 z = dvec2(0.0);
    float i;
    
    for (i = 0.0; i < MAX_ITER; i++) {
        z = dvec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + position;
        if (dot(z, z) > 10.0) break;
    }
    if(i>=MAX_ITER){
        return 0.;
    }
    i = i - log2(log2(dot(vec2(z), vec2(z)))) + 4.0;
    return float(i);
}

vec3 coloring(float i) {
    return 0.5 * cos(i * libre + color) + 0.5;
}

void main() {
    int pixelIndex = int(gl_FragCoord.x) + int(iResolution.x) * int(gl_FragCoord.y);

    int frame = int(colBuff[pixelIndex].w);
    g_seed = float(base_hash(floatBitsToUint(gl_FragCoord.xy))) / float(0xffffffffU) + float(iTime.x+pow(iPass, 3));

    vec2 uv = iFragPos;
    uv.x*=(iResolution.x/iResolution.y);
    uv *= rot(angle);
    uv+=(2.*hash2(g_seed)-1.)/500.;
    //dvec2 scaledPos = dvec2(uv) * pow(zoom, 16.0) + pos;
    dvec2 scaledPos2 = dvec2(uv) * pow(zoom, 16.0) + dvec2(-0.7615760010062,-0.0847596);

    float d = mandelbrot(scaledPos2);
    vec3 color = coloring(d);

    if(d==0){
        color = vec3(0);
    }


    if(iPass == 0)
        colBuff[pixelIndex] = vec4(color,1);
    else
        colBuff[pixelIndex] += vec4(color,1);

    //FragColor = vec4(colBuff[pixelIndex].xyz/colBuff[pixelIndex].w, 1.0);
    FragColor = vec4(colBuff[pixelIndex].xyz/colBuff[pixelIndex].w, 1.0);
}
