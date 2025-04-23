#version 460

#include "C:/Users/Gerome/source/repos/Seden-Animation/Seden/assets/shaders/hash.glsl"

layout(std430, binding = 0) buffer _colBuff {
   vec4 colBuff[]; 
};

layout(std430, binding = 1) buffer _arrayBuff {
   float arrayBuff[]; 
};

out vec4 FragColor;
in vec2 iFragPos;
uniform vec2 iResolution;
uniform vec2 iTime;
uniform int iPass;

uniform float zoom;
uniform float libre;
uniform float power;
uniform vec2 pos;
uniform vec3 color;
uniform vec3 m_precision;

#define MAX_FLOAT 9.e+30
#define MAX_ITER 1000

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct HitRecord{
    vec3 position;
    float dist;
    vec3 normal;
    vec3 color;
};

vec2 random_in_unit_disk(inout float seed) {
    vec2 h = hash2(seed) * vec2(1.,6.28318530718);
    float phi = h.y;
    float r = sqrt(h.x);
	return r * vec2(sin(phi),cos(phi));
}
float sdMandelbulb(vec3 pos, out float t0) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
    t0 = MAX_FLOAT;
	for (int i = 0; i < 12. ; i++) {
		r = length(z);
		if(r > 1.5) break;
		
		// convert to polar coordinates
		float theta = atan(z.y,z.x);
		float phi = asin(z.z/r);
		dr =  pow( r, power-1.0)*power*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,power);
		theta = theta*power;
		phi = phi*power;
		
		// convert back to cartesian coordinates
		z = zr*vec3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
		z+=pos;
        
        t0 = min(t0, zr);
	}
	return 0.5*log(r)*r/dr;
}
float sdScene(vec3 p, out float t0){
    float h = MAX_FLOAT;
    h=min(h,sdMandelbulb(p, t0));
    //h=max(p.z, h);
    return h;
}
vec3 calcNormal(in vec3 p) {
    vec2 e = vec2(1.0, -1.0) * 0.000005;
    float tmp;
    return normalize(
      e.xyy * sdScene(p + e.xyy,tmp) +
      e.yyx * sdScene(p + e.yyx,tmp) +
      e.yxy * sdScene(p + e.yxy,tmp) +
      e.xxx * sdScene(p + e.xxx,tmp));
}
vec3 setFrontNormal(vec3 n, vec3 dir){
    return dot(n,dir)<0 ? n : -n;
}
bool rayMarch(Ray ray, inout HitRecord hit){
    float minDist = m_precision.x;
    float maxDist = m_precision.y;

    float totalDistance = 0.;
    int i;
    vec3 p;
    float t0 = 0.;
    for(i=0; i<255; i++){
        p = ray.origin + totalDistance*ray.direction;
        float currentDistance = sdScene(p, t0);
        totalDistance += currentDistance;
        
        if(currentDistance < minDist || totalDistance > maxDist){
            totalDistance-=minDist;
            break;
        }
    }
    
   
    if(totalDistance>maxDist){
        return false;
    }
    hit.dist = totalDistance;
    p = ray.origin + totalDistance*ray.direction;
    hit.position = ray.origin + 0.99*totalDistance*ray.direction;
    hit.normal = setFrontNormal(calcNormal(p),ray.direction);

    //hit.color =  0.5 + 0.5 * sin(fractalOptions.shift + fractalOptions.frequency * t0 + fractalOptions.color);
    hit.color =  0.5 + 0.5 * sin(libre*t0*color);
    return true;
}
vec3 getSkyColor(vec3 direction){
    vec3 dir = normalize(direction);
    vec3 sunDir = normalize(vec3(1,1,1));
    float v = max(0.,dot(dir, sunDir));
    // background
    vec3 color = mix(vec3(1,0.9,1),vec3(0.7,0.9,1), direction.y);
    // sun bloom
    color += vec3(v)*0.2;
    // sun
    color += pow(v,400.);
    return color;
}
float softShadow(vec3 ro,vec3 rd, float mint, float maxt, float w ){
    float res = 1.0;
    float t = mint;
    float tmp;
    for( int i=0; i<256 && t<maxt; i++ )
    {
        float h = sdScene(ro + t*rd,tmp);
        res = min( res, h/(w*t) );
        t += clamp(h, 0.005, 0.50);
        if( res<-1.0 || t>maxt ) break;
    }
    res = max(res,-1.0);
    return 0.25*(1.0+res)*(1.0+res)*(2.0-res);
    // https://iquilezles.org/articles/rmshadows/
}
float ambientOcclusion(vec3 point, vec3 normal, float step_dist, float step_nbr){
    float occlusion = 1.f;
    float tmp;
    while(step_nbr > 0.0)
    {
	occlusion -= pow(step_nbr * step_dist - (sdScene( point + normal * step_nbr * step_dist,tmp)),2.) / step_nbr;
	step_nbr--;
    }

    return occlusion;
}
vec3 fog(vec3 color, vec3 background, float dist){
    return mix(color, background, 1.-exp(-0.2 * dist * dist));
}

vec3 blinnPhong(Ray ray){
    HitRecord hit;
    vec3 color = getSkyColor(ray.direction);
    vec3 lightPosition = vec3(1,1,1)*1000.;
    if(rayMarch(ray, hit)){
        vec3 lightDirection = normalize(lightPosition - hit.position);
        
        float ambient = 0.5;
        float diffuse = max(dot(hit.normal, lightDirection),0.)*0.5;
        vec3 halfwayDir = normalize(lightDirection + (ray.origin - hit.position));  
        float spec = pow(max(dot(hit.normal, halfwayDir), 0.0), 32.0);

        
        float sh = max(0.1,softShadow(hit.position, lightDirection, 0.01, 2.0, 0.001));
        float ambOccl = clamp(pow(ambientOcclusion(hit.position,hit.normal,0.02,20.),64.),0.3,1.);

        color = hit.color+0.7;
        color = (ambient*ambOccl+diffuse)*color*sh;

        color = fog(color, getSkyColor(ray.direction), hit.dist);
    }
    
    return color;
}
Ray getRay(vec2 uv, vec3 origin, vec3 direction){
    float defocusAngle =  0.;
    float focusDistance = arrayBuff[0];
    
    float vfov = 90.; //vertical field of view
    
    float h = focusDistance * tan(radians(vfov)/2.); // half of vertical sensor size
    
    vec3 fixedUp = vec3(0,1,0);
    vec3 front = normalize(direction);
    vec3 right = -normalize(cross(front, fixedUp));
    vec3 up = cross(front, right);  
    
    float defocusRadius = focusDistance * tan(radians(defocusAngle/2.));
    
    vec2 rd = random_in_unit_disk(g_seed)*defocusRadius;
    vec3 offset = rd.x * right + rd.y * up;
        
    vec2 aa = hash2(g_seed)*0.003; // antialiasing

    vec3 dir = normalize(h*(uv.x+aa.x)*right + h*(uv.y+aa.y)*up + front*focusDistance - offset);
    return Ray(origin+offset, dir);

}

float vignette(vec2 uv){
    // from https://www.shadertoy.com/view/lsKSWR
    uv *=  1.0 - uv.yx;
    return pow(uv.x*uv.y *30.0, 0.07);
}

void main() {
    int pixelIndex = int(gl_FragCoord.x) + int(iResolution.x) * int(gl_FragCoord.y);

    if(iTime.x == 0){
        // init
        arrayBuff[0] = 2.;
    }

    g_seed = float(base_hash(floatBitsToUint(gl_FragCoord.xy))) / float(0xffffffffU) + float(iTime.x+pow(iPass, 3));

    vec2 uv = iFragPos;
    uv.x*=(iResolution.x/iResolution.y);
        
    vec3 o = arrayBuff[0] * vec3(
        sin(pos.y) * cos(pos.x),  
        cos(pos.y),               
        sin(pos.y) * sin(pos.x)
    );

    Ray r = getRay(uv, o, normalize(-o));

    float placeHolder;
    float speed = min(sdScene(o, placeHolder), 0.01)*zoom;
    arrayBuff[0] -= speed * iTime.y;

    vec3 color = blinnPhong(r);

    if(iPass == 0)
        colBuff[pixelIndex] = vec4(color,1);
    else
        colBuff[pixelIndex] += vec4(color,1);

    FragColor = vec4(colBuff[pixelIndex].rgb/colBuff[pixelIndex].a, 1.0);
}