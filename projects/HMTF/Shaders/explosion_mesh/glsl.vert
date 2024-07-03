#version 300 es
precision lowp int;
precision lowp int;
precision lowp float;

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
uniform int render_no;
uniform float progresion;

vec3 randomDirection(int seed) {
    float angle1 = float(seed) * 43758.5453123;
    float angle2 = float(seed) * 24634.634634;
    float x = cos(angle1) * sin(angle2);
    float y = sin(angle1) * sin(angle2);
    float z = cos(angle2);
    return normalize(vec3(x, y, z));
}


void main() {

   vert.POS = vec4(position, 1.0f);
   vert.UV = uv;
   vert.COLOR = color;
   vert.NORMAL_COLOR = normal * 0.5f + 0.5f;

   if (render_no != 0) {
       vec3 randomDir = randomDirection(render_no);
       vert.POS.xyz += randomDir * progresion;
   }

   if(ui) {
      gl_Position = transform * vert.POS;
   } else {
      gl_Position = (projection * vision * transform) * vert.POS;
   }

   // Quantização para efeito de estilo pixelado
   int psx_factor = 16;
   gl_Position.x = (floor(gl_Position.x * float(psx_factor) + 0.5f)) / float(psx_factor);
   gl_Position.y = (floor(gl_Position.y * float(psx_factor) + 0.5f)) / float(psx_factor);
}
