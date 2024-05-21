#version 330 core


out vec4 pos;
out vec2 uv;

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
   
   pos = vec4( quad_data[gl_VertexID].x / 2,-quad_data[gl_VertexID].y / 2,quad_data[gl_VertexID].z / 2,1);
   
   
   uv = vec2(max(0,quad_data[gl_VertexID].x),max(0,quad_data[gl_VertexID].y));

   //gl_position = pos;
   
   if(ui){
   gl_Position =  transform * pos; 
   }else{
   gl_Position = (projection * vision * transform) * pos ; 
   }
   
}




