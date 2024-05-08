#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;

out Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
} vert_out;

uniform mat4 matrix;

void main() {

   vert_out.POS = vec4(position, 1);
   vert_out.UV = uv;
   vert_out.COLOR = color;

   
   gl_Position = matrix * vert_out.POS;

}
