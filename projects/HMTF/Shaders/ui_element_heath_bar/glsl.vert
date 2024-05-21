#version 330 core


vec3 quad_data[4] = vec3[4](vec3(1, -1, 0), vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0));

out vec4 pos;
out vec2 uv;

/*
uniform bool is_3D;
uniform mat4 matrix;
uniform vec3 position;
uniform vec2 scale;
uniform float roundnes;
uniform float skew;
uniform float border_size;
uniform vec4 color;
uniform vec4 border_color;
*/

uniform mat4 matrix;
uniform float skew;

void main() {
  //tela

  vec3 q = quad_data[gl_VertexID];

  uv = vec2(max(0, q.x), max(0, q.y));

  if(q.y == 1){
    q.x += skew / 2;
  }else if(q.y == -1){
    q.x -= skew / 2;
  }

  pos = vec4(q, 1);

  gl_Position = matrix * pos;



}
