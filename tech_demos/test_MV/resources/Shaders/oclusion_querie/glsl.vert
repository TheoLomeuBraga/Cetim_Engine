#version 460 core

layout(location = 0) in vec3  position;

uniform bool ui;
uniform mat4 projection,vision,transform;



  
 
 void main(){
   
   if(ui){
   gl_Position =  transform * vec4(position,1); 
   }else{
   gl_Position = (projection * vision * transform) * vec4(position,1); 
   }




   
}




