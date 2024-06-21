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

const int MAX_BONES = 256;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool skin_mode;

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

   vert.POS = vec4(position, 1.0);
   vert.UV = uv;
   vert.COLOR = color;
   vert.NORMAL_COLOR = normal * 0.5 + 0.5;

   if (skin_mode) {
      mat4 boneTransform = mat4(0.0);
      
      float weights_sum = weights.x + weights.y + weights.z + weights.w;

      for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
         int boneIndex = boneIds[i];
         float weight = weights[i];
         if (weight == 0.0) { break; }
         boneTransform += finalBonesMatrices[boneIndex] * weight;
      }

      if (ui) {
         gl_Position = boneTransform * vert.POS;
      } else {
         gl_Position = (projection * vision * boneTransform) * vert.POS;
      }
      
   } else {
      if (ui) {
         gl_Position = transform * vert.POS;
      } else {
         gl_Position = (projection * vision * transform) * vert.POS;
      }
   }

   // Quantização para efeito de estilo pixelado
   int psx_factor = 128;
   gl_Position.x = (floor(gl_Position.x * float(psx_factor) + 0.5)) / float(psx_factor);
   gl_Position.y = (floor(gl_Position.y * float(psx_factor) + 0.5)) / float(psx_factor);
}
