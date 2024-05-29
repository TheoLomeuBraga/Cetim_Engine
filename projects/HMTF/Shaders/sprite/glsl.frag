#version 300 es

precision lowp int;
precision lowp float;

in vec4 pos;
in vec2 uv;

layout(location = 0) out vec4 ret;

// material
 uniform sampler2D textures[6];
uniform vec4 color;
uniform vec4 uv_position_scale;

//fun�oes
vec2 re_pos_uv(vec2 UV,vec4 UV_PosSca){
return UV * UV_PosSca.zw + UV_PosSca.xy;
}

void main(){

    ret = color * texture(textures[0],(uv * uv_position_scale.zw) + uv_position_scale.xy);
}