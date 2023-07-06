#version 330 core
#extension GL_ARB_separate_shader_objects : require



layout(location = 0) out vec4 POS;
layout(location = 1) out vec2 UV;











//triangulo










//Vertices












vec3 quad_data[4] = vec3[4](
  //vec3(1,-1,0),
  //vec3(1,1,0),
  //vec3(-1,-1,0),
  //vec3(-1,1,0)
  
  vec3(1,0,0),
  vec3(1,1,0),
  vec3(0,0,0),
  vec3(0,1,0)
  );




uniform bool ui;
uniform mat4 projection,vision,transform;



  
 
 void main(){
  //tela
   
   POS =vec4( quad_data[gl_VertexID].x,-quad_data[gl_VertexID].y,quad_data[gl_VertexID].z,1);
   
   
   UV= vec2(max(0,quad_data[gl_VertexID].x),max(0,quad_data[gl_VertexID].y));

   //gl_Position = POS;
   
   if(ui){
   gl_Position =  transform * POS; 
   }else{
   gl_Position = (projection * vision * transform) * POS ; 
   }
   




   
}




