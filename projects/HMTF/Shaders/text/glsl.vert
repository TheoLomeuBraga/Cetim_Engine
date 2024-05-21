#version 330 core

out vec4 pos;
out vec2 uv;



vec3 quad_data[4] = vec3[4](
  vec3(1,-1,0),
  vec3(1,1,0),
  vec3(-1,-1,0),
  vec3(-1,1,0)
  );



uniform bool ui;
uniform mat4 projection,vision,transform;



  
 
 void main(){
  //tela
   
   pos =vec4( quad_data[gl_VertexID].x,-quad_data[gl_VertexID].y,quad_data[gl_VertexID].z,1);
   
   
   uv= vec2(max(0,quad_data[gl_VertexID].x),max(0,quad_data[gl_VertexID].y));

   //gl_Position = POS;
   
   if(ui){
   gl_Position =  transform * pos; 
   }else{
   gl_Position = (projection * vision * transform) * pos ; 
   }
   




   
}




