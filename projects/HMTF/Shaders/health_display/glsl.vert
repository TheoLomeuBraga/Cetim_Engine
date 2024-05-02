#version 330 core
#extension GL_ARB_separate_shader_objects : require


layout(location = 0) out vec4 POS;
layout(location = 1) out vec2 UV;

vec3 quad_data[6] = vec3[6](
  vec3(-1,-1,0),
  vec3(1,-1,0),
  vec3(1,1,0),
  
  vec3(1,1,0),
  vec3(-1,1,0),
  vec3(-1,-1,0)
  );


uniform bool ui;
uniform mat4 projection,vision,transform;

 void main(){
  //tela
   
   POS = vec4( quad_data[gl_VertexID].x / 2,-quad_data[gl_VertexID].y / 2,quad_data[gl_VertexID].z / 2,1);
   
   
   UV = vec2(max(0,quad_data[gl_VertexID].x),max(0,quad_data[gl_VertexID].y));

   //gl_Position = POS;
   
   if(ui){
   gl_Position =  transform * POS; 
   }else{
   gl_Position = (projection * vision * transform) * POS ; 
   }
   
}



