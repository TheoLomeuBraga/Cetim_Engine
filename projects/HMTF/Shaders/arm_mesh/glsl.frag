#version 300 es
precision lowp int;
precision lowp float;



layout(location = 0) out vec4 ret;

struct Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
   vec3 NORMAL_COLOR;
};

in Vertex vert;

// material
uniform sampler2D textures[6];
uniform vec4 color;
uniform vec4 uv_position_scale;
uniform float time;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
   return UV * UV_PosSca.zw + UV_PosSca.xy;
}

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


   vec2 uv_pos = re_pos_uv(vert.UV, uv_position_scale);

   vec2 uv_pos_noise = re_pos_uv(vert.UV, vec4(time * 5.0,time * 5.0,100.0,100.0));
   float perlin_f = mix(0.9,1.0,perlin(uv_pos_noise, 1.0));
   vec4 perlin_v4 = vec4(perlin_f,perlin_f,perlin_f,1.0);
   vec4 spel_color = color * perlin_v4;

   

   if(color == vec4(1.0,0.0,0.0,1.0)){

      float perlin_f = perlin(uv_pos_noise, 2.0);
      vec4 perlin_v4 = vec4(perlin_f,perlin_f,perlin_f,1.0);

      spel_color = mix(vec4(1.0,0.5,0.0,1.0),color,perlin_v4);
   }

   ret = mix(texture(textures[0], uv_pos),spel_color,vert.COLOR.x);


}