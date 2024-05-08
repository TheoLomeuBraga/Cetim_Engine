#version 330 core
#extension GL_ARB_separate_shader_objects : require


layout(location = 0) out vec4 POS;
layout(location = 1) out vec2 UV;



vec3 quad_data[4] = vec3[4](
  vec3(1,-1,0),
  vec3(1,1,0),
  vec3(-1,-1,0),
  vec3(-1,1,0)
  );



uniform bool ui;
uniform mat4 matrix;



  
 
 void main(){
  //tela
   
   
   
   
   UV= vec2(max(0,quad_data[gl_VertexID].x),max(0,quad_data[gl_VertexID].y));

   //gl_Position = POS;
  
  POS =vec4( quad_data[gl_VertexID].x,-quad_data[gl_VertexID].y,quad_data[gl_VertexID].z,1);
  gl_Position =  matrix * POS; 
   




   
}




