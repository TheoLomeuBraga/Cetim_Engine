#version 300 es
precision lowp int;
precision lowp float;

vec3 quad_data[4] = vec3[4](vec3(1.0, -1.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(-1.0, -1.0, 0.0), vec3(-1.0, 1.0, 0.0));

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
    // Calcular a posição
    vec3 q = quad_data[gl_VertexID];

    // Calcular as coordenadas de textura
    uv = vec2(max(0.0, q.x), max(0.0, q.y));

    // Ajustar a posição com skew
    if (q.y == 1.0) {
        q.x += skew / 2.0;
    } else if (q.y == -1.0) {
        q.x -= skew / 2.0;
    }

    pos = vec4(q, 1.0);

    gl_Position = matrix * pos;
}
