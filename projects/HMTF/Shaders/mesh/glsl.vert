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



const int MAX_BONES = 256;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool skin_mode;

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

   if(skin_mode) {

      mat4 boneTransform = mat4(0.0);
      
      float weights_sum = weights.x + weights.y + weights.z + weights.w;

      for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
         int boneIndex = boneIds[i];
         float weight = weights[i];
         if(weight == 0){break;}
         boneTransform += finalBonesMatrices[boneIndex] * weight;
      }

      //boneTransform = finalBonesMatrices[214];

      if(ui) {
         gl_Position = boneTransform * vert_out.POS;
      } else {
         gl_Position = (projection * vision * boneTransform) * vert_out.POS;
      }
      
   } else {
      if(ui) {
         gl_Position = transform * vert_out.POS;
      } else {
         gl_Position = (projection * vision * transform) * vert_out.POS;
      }
   }
   
   
   int psx_factor = 16;
   gl_Position.x = (float(int(gl_Position.x * psx_factor) + 0.5)) / psx_factor;
   gl_Position.y = (float(int(gl_Position.y * psx_factor) + 0.5)) / psx_factor;
   //vert_out.UV.x = (float(int(vert_out.UV.x * psx_factor) + 0.5)) / psx_factor;
   //vert_out.UV.y = (float(int(vert_out.UV.y * psx_factor) + 0.5)) / psx_factor;
   

}
