#version 330 core
#extension GL_ARB_separate_shader_objects : require
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 ret;

// material
uniform sampler2D post_procesing_render_input[6];
uniform sampler2D textures[6];
uniform vec4 color;
uniform float gama;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
    return UV * UV_PosSca.zw + UV_PosSca.xy;
}



void main() {

    ret = color * texture(post_procesing_render_input[0], uv);

    //limit color pallet
    const float numBits = 4.0;
    ret = floor(ret * (pow(2.0, numBits) - 1.0)) / (pow(2.0, numBits) - 1.0);
    /*
    int n = 8;
    ret.x = (float(int(ret.x * n) + 0.5)) / n;
    ret.y = (float(int(ret.y * n) + 0.5)) / n;
    ret.z = (float(int(ret.z * n) + 0.5)) / n;
    ret.w = (float(int(ret.w * n) + 0.5)) / n;
    */
    

}