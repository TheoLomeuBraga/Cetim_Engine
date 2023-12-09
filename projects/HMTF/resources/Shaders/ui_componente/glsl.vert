#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) out vec4 POS;
layout(location = 1) out vec2 UV;

vec3 top_side_box_data[6] = vec3[6](vec3(0, 0, 0), vec3(2, 0, 0), vec3(2, 2, 0), vec3(2, 2, 0), vec3(0, 2, 0), vec3(0, 0, 0));

uniform bool ui;
uniform mat4 projection, vision, transform;

void main() {
  //tela

  POS = vec4(top_side_box_data[gl_VertexID].x, -top_side_box_data[gl_VertexID].y, top_side_box_data[gl_VertexID].z, 1);

  UV = vec2(top_side_box_data[gl_VertexID].x, top_side_box_data[gl_VertexID].y);

   //gl_Position = POS;

  if(ui) {
    gl_Position = transform * POS;
  } else {
    gl_Position = (projection * vision * transform) * POS;
  }

}
