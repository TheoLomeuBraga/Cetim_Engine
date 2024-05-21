#version 300 es
precision mediump float;

in vec4 pos;
in vec2 uv;

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

void main() {

    //float border_size = inputs[0] / 2;
    //vec4 border_color = vec4(inputs[1],inputs[2],inputs[3],inputs[4]);

    
    ret = color * texture(textures[0], re_pos_uv(uv, uv_position_scale));
}