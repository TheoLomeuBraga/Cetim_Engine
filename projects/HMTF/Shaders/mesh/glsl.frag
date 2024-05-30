#version 300 es
precision lowp int;
precision lowp float;



layout(location = 0) out vec4 ret;

struct Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
   vec3 NORMAL_COLOR;
};

in Vertex vert;

// material
uniform sampler2D textures[6];
uniform vec4 color;
uniform vec4 uv_position_scale;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
   return UV * UV_PosSca.zw + UV_PosSca.xy;
}

void main() {

   
   ret = color * texture(textures[0], re_pos_uv(vert.UV, uv_position_scale)) * vec4(vert.COLOR.x,vert.COLOR.y,vert.COLOR.z,1);
   /*
   if(ret.w == 0.0){
      discard;
   }
   */

}