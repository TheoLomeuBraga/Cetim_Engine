#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) out vec4 ret;

in Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
} vert_in;

// material
uniform sampler2D image;
uniform vec4 color;

void main() {

   
   //ret = color * texture(image, vert_in.UV) * vec4(vert_in.COLOR.x,vert_in.COLOR.y,vert_in.COLOR.z,1);
   ret = vec4(0,0,0,1);
}