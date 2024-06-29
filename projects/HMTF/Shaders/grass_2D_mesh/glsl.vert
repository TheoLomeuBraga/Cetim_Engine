#version 300 es
precision lowp int;
precision lowp int;
precision lowp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;
layout(location = 5) in vec3 normal;
layout(location = 6) in vec3 tangent;
layout(location = 7) in vec3 bitangents;

const int MAX_BONES = 256;
const int MAX_BONE_INFLUENCE = 2;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool skin_mode;

struct Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
   vec3 NORMAL_COLOR;
};
out Vertex vert;

uniform bool ui;
uniform mat4 projection, vision, transform;
uniform float time;


float M_PI = 3.14159265358979323846;

float rand(vec2 co){return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);}
float rand (vec2 co, float l) {return rand(vec2(rand(co), l));}
float rand (vec2 co, float l, float t) {return rand(vec2(rand(co, l), t));}

float perlin(vec2 p, float dim, float time) {
	vec2 pos = floor(p * dim);
	vec2 posx = pos + vec2(1.0, 0.0);
	vec2 posy = pos + vec2(0.0, 1.0);
	vec2 posxy = pos + vec2(1.0);
	
	float c = rand(pos, dim, time);
	float cx = rand(posx, dim, time);
	float cy = rand(posy, dim, time);
	float cxy = rand(posxy, dim, time);
	
	vec2 d = fract(p * dim);
	d = -0.5 * cos(d * M_PI) + 0.5;
	
	float ccx = mix(c, cx, d.x);
	float cycxy = mix(cy, cxy, d.x);
	float center = mix(ccx, cycxy, d.y);
	
	return center * 2.0 - 1.0;
}

// p must be normalized!
float perlin(vec2 p, float dim) {
	return perlin(p, dim, 0.0);
}


void main() {

   vert.POS = vec4(position, 1.0);
   vert.UV = uv;
   vert.COLOR = color;
   vert.NORMAL_COLOR = normal * 0.5 + 0.5;

   vert.POS.xz +=  mix(0.0,0.1,perlin(vert.POS.xz * time, 1.0)) * vert.POS.y ;
   

   if (skin_mode) {
      mat4 boneTransform = mat4(0.0);
      
      float weights_sum = weights.x + weights.y + weights.z + weights.w;

      for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
         int boneIndex = boneIds[i];
         float weight = weights[i];
         if (weight == 0.0) { break; }
         boneTransform += finalBonesMatrices[boneIndex] * weight;
      }

      if (ui) {
         gl_Position = boneTransform * vert.POS;
      } else {
         gl_Position = (projection * vision * boneTransform) * vert.POS;
      }
      
   } else {
      if (ui) {
         gl_Position = transform * vert.POS;
      } else {
         gl_Position = (projection * vision * transform) * vert.POS;
      }
   }

   int psx_factor = 16;
   gl_Position.x = (floor(gl_Position.x * float(psx_factor) + 0.5)) / float(psx_factor);
   gl_Position.y = (floor(gl_Position.y * float(psx_factor) + 0.5)) / float(psx_factor);

   
}
