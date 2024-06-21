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

uniform float color_2_r;
uniform float color_2_g;
uniform float color_2_b;
uniform float perlin_speed;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
   return UV * UV_PosSca.zw + UV_PosSca.xy;
}

float M_PI = 3.14159265358979323846f;

float rand(vec2 co) {
   return fract(sin(dot(co.xy, vec2(12.9898f, 78.233f))) * 43758.5453f);
}
float rand(vec2 co, float l) {
   return rand(vec2(rand(co), l));
}
float rand(vec2 co, float l, float t) {
   return rand(vec2(rand(co, l), t));
}

float perlin(vec2 p, float dim, float time) {
   vec2 pos = floor(p * dim);
   vec2 posx = pos + vec2(1.0f, 0.0f);
   vec2 posy = pos + vec2(0.0f, 1.0f);
   vec2 posxy = pos + vec2(1.0f);

   float c = rand(pos, dim, time);
   float cx = rand(posx, dim, time);
   float cy = rand(posy, dim, time);
   float cxy = rand(posxy, dim, time);

   vec2 d = fract(p * dim);
   d = -0.5f * cos(d * M_PI) + 0.5f;

   float ccx = mix(c, cx, d.x);
   float cycxy = mix(cy, cxy, d.x);
   float center = mix(ccx, cycxy, d.y);

   return center * 2.0f - 1.0f;
}

// p must be normalized!
float perlin(vec2 p, float dim) {
   return perlin(p, dim, 0.0f);
}

void main() {

   vec2 uv_pos = re_pos_uv(vert.UV, uv_position_scale);

   vec2 uv_pos_noise = re_pos_uv(vert.UV, vec4(time * perlin_speed, time * perlin_speed, 75.0f,75.0f));

   float perlin_f = perlin(uv_pos_noise, 1.5f);
   vec4 perlin_v4 = vec4(perlin_f, perlin_f, perlin_f, 2.0f);

   vec4 spel_color = mix(color,vec4(color_2_r, color_2_g, color_2_b, 1.0f), perlin_v4);

   ret = mix(texture(textures[0], uv_pos), spel_color, vert.COLOR.x);

}