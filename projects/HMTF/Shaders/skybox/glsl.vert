#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) out vec4 POS;
layout(location = 1) out vec2 UV;

mat4 PVT;
uniform float tempo;

//triangulo
uniform vec3[3] vertice_triangulo;
uniform vec2[3] uv_triangulo;

uniform int lod;

//Vertices

vec3 quad_data[6] = vec3[6](vec3(-1, -1, 0), vec3(1, -1, 0), vec3(1, 1, 0), vec3(1, 1, 0), vec3(-1, 1, 0), vec3(-1, -1, 0));

uniform mat4 projection, vision, transform;

void main() {
  //tela

  POS = vec4(quad_data[gl_VertexID], 1);

  UV = vec2(max(0, quad_data[gl_VertexID].x), -max(0, quad_data[gl_VertexID].y));

  gl_Position = POS;

    // Transforma a posição para o espaço do mundo
  vec4 worldPos = inverse(vision) * inverse(projection) * POS;
  worldPos /= worldPos.w; // Normaliza para converter para coordenadas de direção

    // Configura as saídas
  UV = worldPos.xy;     // Usa as coordenadas XYZ como direção para amostrar a textura da skybox

}
