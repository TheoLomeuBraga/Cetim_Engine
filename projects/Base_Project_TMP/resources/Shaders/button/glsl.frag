#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 ret;










// material
uniform sampler2D textures[12];
uniform float inputs[32];
uniform vec4 color;
uniform vec4 uv_position_scale;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
   return UV * UV_PosSca.zw + UV_PosSca.xy;
}

float isInCenter(vec2 uv, float borderSize, float roundness) {
    // Calculate the distance from the center of the UV coordinates
    vec2 center = vec2(0.5, 0.5);
    vec2 dist = abs(uv - center) - (1.0 - roundness) * borderSize;

    // Calculate the distance from the border edge
    float borderDistance = length(max(dist, 0.0));

    // Check if the point is in the center range or border range
    return step(borderSize, borderDistance);
}

void main(){

    float border_size = inputs[0];
    float border_roundnes = inputs[1];
    vec4 border_color = vec4(inputs[2],inputs[3],inputs[4],inputs[5]); 

    //ret = color * texture(textures[0], re_pos_uv(uv, uv_position_scale));

    ret = mix(color,border_color,isInCenter(uv, 0.4,0.5));
  
}