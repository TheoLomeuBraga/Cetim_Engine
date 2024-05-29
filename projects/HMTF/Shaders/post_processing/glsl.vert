#version 300 es
precision lowp float;


out vec4 pos;
out vec2 uv;

mat4 PVT;
uniform float tempo;

//triangulo
uniform vec3[3] vertice_triangulo;
uniform vec2[3] uv_triangulo;

uniform int lod;

//Vertices

//vec3 quad_data[6] = vec3[6](
//  vec3(-1,-1,0),
//  vec3(1,-1,0),
//  vec3(1,1,0),
//  
//  vec3(1,1,0),
//  vec3(-1,1,0),
//  vec3(-1,-1,0)
//  );

vec3 quad_data[4] = vec3[4](vec3(1, -1, 0), vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0));

void main() {
  //tela

  pos = vec4(quad_data[gl_VertexID], 1);

  uv = vec2(max(0.0, quad_data[gl_VertexID].x), max(0.0, quad_data[gl_VertexID].y));

  gl_Position = pos;

}
