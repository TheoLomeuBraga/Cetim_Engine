#version 300 es
precision mediump float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;
layout(location = 5) in vec3 normal;
layout(location = 6) in vec3 tangent;
layout(location = 7) in vec3 bitangents;

struct Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
   vec3 NORMAL_COLOR;
};

out Vertex vert;



uniform bool ui;
uniform mat4 projection, vision, transform;

void main() {

   vert.POS = vec4(position, 1);
   vert.UV = uv;
   vert.COLOR = color;
   vert.NORMAL_COLOR = normal * 0.5 + 0.5;

   vec4 position = vec4(0,0,0,1);

   mat4 tf = transform;
   tf[3] = position;

   mat4 v = vision;
   v[3] = position;

   gl_Position = (projection * v * tf) * vert.POS;

}
