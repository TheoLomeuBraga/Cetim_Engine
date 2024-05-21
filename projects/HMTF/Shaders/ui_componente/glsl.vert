#version 300 es
precision mediump float;

out vec4 pos;
out vec2 uv;

vec3 top_side_box_data[6] = vec3[6](
    vec3(0.0, 0.0, 0.0), 
    vec3(1.0, 0.0, 0.0), 
    vec3(1.0, 1.0, 0.0), 
    vec3(1.0, 1.0, 0.0), 
    vec3(0.0, 1.0, 0.0), 
    vec3(0.0, 0.0, 0.0)
);

uniform bool ui;
uniform mat4 projection, vision, transform;

void main() {
    // Calcular a posição
    pos = vec4(top_side_box_data[gl_VertexID].x * 2.0, -top_side_box_data[gl_VertexID].y * 2.0, top_side_box_data[gl_VertexID].z, 1.0);

    uv = vec2(top_side_box_data[gl_VertexID].x, top_side_box_data[gl_VertexID].y);

    // Definir gl_Position
    if (ui) {
        gl_Position = transform * pos;
    } else {
        gl_Position = projection * vision * transform * pos;
    }
}
