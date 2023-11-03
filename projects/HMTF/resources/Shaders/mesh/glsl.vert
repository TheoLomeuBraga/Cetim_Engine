#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

const int MAX_BONES = 256;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool skin_mode;

out Vertex {
   vec4 POS;
   vec2 UV;
} vert_out;

uniform bool ui, poly_mesh;
uniform mat4 projection, vision, transform;

void main() {

   vert_out.POS = vec4(position, 1);
   vert_out.UV = uv;

   if(skin_mode) {
      mat4 skin_transform;

      //test
      skin_transform = finalBonesMatrices[0];

      if(ui) {
         gl_Position = skin_transform * vert_out.POS;
      } else {
         gl_Position = (projection * vision * skin_transform) * vert_out.POS;
      }
   } else {
      if(ui) {
         gl_Position = transform * vert_out.POS;
      } else {
         gl_Position = (projection * vision * transform) * vert_out.POS;
      }
   }

   //psx factor
   int psx_factor = 8;
   gl_Position.x = (float(int(gl_Position.x * psx_factor) + 0.5)) / psx_factor;
   gl_Position.y = (float(int(gl_Position.y * psx_factor) + 0.5)) / psx_factor;
   vert_out.UV.x = (float(int(vert_out.UV.x * psx_factor) + 0.5)) / psx_factor;
   vert_out.UV.y = (float(int(vert_out.UV.y * psx_factor) + 0.5)) / psx_factor;

}
