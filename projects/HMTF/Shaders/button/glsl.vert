#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) out vec4 POS;
layout(location = 1) out vec2 UV;

vec3 top_side_box_data[6] = vec3[6](
    vec3(0, 0, 0),
    vec3(1, 0, 0),
    vec3(1, 1, 0),
    vec3(1, 1, 0),
    vec3(0, 1, 0),
    vec3(0, 0, 0)
);

uniform bool ui;
uniform mat4 projection, view, transform;

void main() {
    // Configuração da posição e UV com base em top_side_box_data
    POS = vec4(top_side_box_data[gl_VertexID].x, -top_side_box_data[gl_VertexID].y, top_side_box_data[gl_VertexID].z, 1);
    UV = vec2(top_side_box_data[gl_VertexID].x, top_side_box_data[gl_VertexID].y);

    if (ui) {
        // Modo UI: Usa transformação direta
        gl_Position = transform * POS;
    } else {
        // Modo padrão: Billboarding

        // Extraia a posição da câmera da matriz de visão
        vec3 cameraPos = vec3(view[3]);

        // Calcule a direção para a câmera
        vec3 direction = normalize(cameraPos - vec3(transform[3]));

        // Calcule o vetor "up" e o vetor "right" para o objeto
        vec3 up = vec3(0.0, 1.0, 0.0); // assumindo que o "up" é global
        vec3 right = normalize(cross(up, direction));

        // Recalcule o vetor "up"
        up = cross(direction, right);

        // Construa uma nova matriz de transformação que olha para a câmera
        mat4 billboardTransform = mat4(
            vec4(right, 0.0),
            vec4(up, 0.0),
            vec4(direction, 0.0),
            transform[3]
        );

        // Aplica as transformações
        gl_Position = projection * view * billboardTransform * POS;
    }
}
