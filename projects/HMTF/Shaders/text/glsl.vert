#version 300 es
precision mediump float;

out vec4 pos;
out vec2 uv;

vec3 quad_data[4] = vec3[4](
  vec3(1.0, -1.0, 0.0),
  vec3(1.0, 1.0, 0.0),
  vec3(-1.0, -1.0, 0.0),
  vec3(-1.0, 1.0, 0.0)
);

uniform bool ui;
uniform mat4 projection, vision, transform;

void main() {
  // Calcula a posição
  pos = vec4(quad_data[gl_VertexID].x, -quad_data[gl_VertexID].y, quad_data[gl_VertexID].z, 1.0);

  // Calcula as coordenadas de textura
  uv = vec2(max(0.0, quad_data[gl_VertexID].x), max(0.0, quad_data[gl_VertexID].y));

  // Define gl_Position
  if (ui) {
    gl_Position = transform * pos;
  } else {
    gl_Position = projection * vision * transform * pos;
  }
}
