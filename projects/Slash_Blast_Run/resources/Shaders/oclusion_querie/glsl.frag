#version 330 core
#extension GL_ARB_separate_shader_objects : require
layout(location = 0) out vec4 ret;

void main(){
 ret = vec4(1,0,0,0.1);
}