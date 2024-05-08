#version 330 core
#extension GL_ARB_separate_shader_objects : require

vec3 quad_data[4] = vec3[4](vec3(1, -1, 0), vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0));

layout(location = 0) out vec4 POS;
layout(location = 1) out vec2 UV;

uniform bool is_3D;
uniform mat4 matrix;
uniform vec3 position;
uniform vec2 scale;

void main() {
  //tela

  vec3 q = quad_data[gl_VertexID];

  POS = vec4(q, 1);

  UV = vec2(max(0, q.x), max(0, q.y));

  gl_Position = matrix * POS;
  //gl_Position = POS;



}
