#version 330 core
#extension GL_ARB_separate_shader_objects : require
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

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



    ret = color;


  
}