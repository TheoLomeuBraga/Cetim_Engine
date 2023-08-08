#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec3  position;

uniform bool ui;
uniform mat4 projection,vision,transform;


vec3 quad_data[4] = vec3[4](
  vec3(1,-1,0),
  vec3(1,1,0),
  vec3(-1,-1,0),
  vec3(-1,1,0)
  );
  
 
 void main(){
   
   if(ui){
   gl_Position =  transform * vec4(position,1); 
   }else{
   gl_Position = (projection * vision * transform) * vec4(position,1); 
   }




   
}




