#version 300 es
precision lowp float;

out vec4 pos;
out vec2 uv;

vec3 quad_data[4] = vec3[4](
    vec3(1.0, -1.0, 0.0),
    vec3(1.0, 1.0, 0.0),
    vec3(-1.0, -1.0, 0.0),
    vec3(-1.0, 1.0, 0.0)
);

uniform bool ui;
uniform mat4 matrix;

void main() {
    // Calcular as coordenadas de textura
    uv = vec2(max(0.0, quad_data[gl_VertexID].x), max(0.0, quad_data[gl_VertexID].y));

    // Calcular a posição
    pos = vec4(quad_data[gl_VertexID].x, -quad_data[gl_VertexID].y, quad_data[gl_VertexID].z, 1.0);

    // Definir gl_Position
    gl_Position = matrix * pos;
}
