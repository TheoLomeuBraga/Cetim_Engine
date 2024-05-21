#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;


struct Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
   vec3 NORMAL_COLOR;
};
out Vertex vert;

uniform mat4 matrix;

void main() {

   vert.POS = vec4(position, 1);
   vert.UV = uv;
   vert.COLOR = color;

   
   gl_Position = matrix * vert.POS;

}
