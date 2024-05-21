#version 300 es
precision mediump float;
layout(location = 0) out vec4 ret;




// material
uniform sampler2D image;
uniform vec4 color;

void main() {

   
   //ret = color * texture(image, vert_in.UV) * vec4(vert_in.COLOR.x,vert_in.COLOR.y,vert_in.COLOR.z,1);
   ret = vec4(0,0,0,1);
}