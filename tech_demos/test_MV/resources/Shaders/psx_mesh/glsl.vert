#version 460 core


layout(location = 0) in vec3  position;
layout(location = 1) in vec2  uv;


out Vertex{
   vec4 POS;
   vec2 UV;
}vert_out;

uniform bool ui;
uniform mat4 projection,vision,transform;


vec3 psxVertexPrecision(vec3 vertexPosition,float precision) {
   vec3 quantizedPosition = round(vertexPosition * precision) / precision;
   return quantizedPosition;
}

vec4 psxVertexPrecision(vec4 vertexPosition,float precision) {
   vec3 quantizedPosition = round(vertexPosition * precision) / precision;
   return vec4(quantizedPosition,vertexPosition.w);
}
 
 void main(){

   vert_out.POS = vec4(position,1);
   vert_out.UV = uv;
   
   if(ui){
   gl_Position =  psxVertexPrecision(transform * vert_out.POS,256.0); 
   }else{
   gl_Position = psxVertexPrecision((projection * vision * transform) * vert_out.POS,256.0); 
   }




   
}




