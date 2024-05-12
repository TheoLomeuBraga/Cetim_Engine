#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;
layout(location = 5) in vec3 normal;
layout(location = 6) in vec3 tangent;
layout(location = 7) in vec3 bitangents;



out Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
   vec3 NORMAL_COLOR;
} vert_out;

uniform bool ui;
uniform mat4 projection, vision, transform;

void main() {

   vert_out.POS = vec4(position, 1);
   vert_out.UV = uv;
   vert_out.COLOR = color;
   vert_out.NORMAL_COLOR = normal * 0.5 + 0.5;

   mat4 tf = transform;
   tf[3][0] = 0.0;
   tf[3][1] = 0.0;
   tf[3][2] = 0.0;
   tf[3][3] = 1.0;

   mat4 v = vision;
   v[3][0] = 0.0;
   v[3][1] = 0.0;
   v[3][2] = 0.0;
   v[3][3] = 1.0;

   gl_Position = (projection * v * tf) * vert_out.POS;

}
