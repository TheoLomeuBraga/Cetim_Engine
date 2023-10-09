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

float checkSquareBorder(vec2 uv, float borderSize) {
    // Calculate half size of the center square
    float centerHalfSize = 0.5 - borderSize;

    // Calculate half size of the border squares
    float borderHalfSize = borderSize;

    // Define the coordinates of the center square
    vec2 centerMin = vec2(0.5 - centerHalfSize);
    vec2 centerMax = vec2(0.5 + centerHalfSize);

    // Define the coordinates of the border squares
    vec2 borderMin = vec2(0.5 - borderHalfSize);
    vec2 borderMax = vec2(0.5 + borderHalfSize);

    // Check if the UV point is within the center square
    if(all(greaterThanEqual(uv, centerMin)) && all(lessThanEqual(uv, centerMax))) {
        return 0.0;  // UV point is in the center square
    } else if(all(greaterThanEqual(uv, borderMin)) && all(lessThanEqual(uv, borderMax))) {
        return 1.0;  // UV point is in the border squares
    } else {
        return 1.0;  // UV point is outside both center and border squares
    }
}

uniform float border_size;
uniform float border_color_x;
uniform float border_color_y;
uniform float border_color_z;
uniform float border_color_w;

void main() {

    //float border_size = inputs[0] / 2;
    //vec4 border_color = vec4(inputs[1],inputs[2],inputs[3],inputs[4]);

    vec4 ret2 = color * texture(textures[0], re_pos_uv(uv, uv_position_scale));

    vec4 border_color = vec4(border_color_x, border_color_y, border_color_z, border_color_w);

    ret = mix(ret2, border_color, checkSquareBorder(uv, border_size / 2));
}