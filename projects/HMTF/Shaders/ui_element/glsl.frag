#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 1) in vec2 UV;
layout(location = 0) out vec4 ret;


uniform vec4 color;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
    return UV * UV_PosSca.zw + UV_PosSca.xy;
}

void main() {

    ret = color;
}