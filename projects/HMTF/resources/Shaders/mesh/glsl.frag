#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) out vec4 ret;

in Vertex {
   vec4 POS;
   vec2 UV;
   vec3 COLOR;
   vec3 NORMAL_COLOR;
} vert_in;

// material
uniform sampler2D textures[6];
uniform vec4 color;
uniform vec4 uv_position_scale;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
   return UV * UV_PosSca.zw + UV_PosSca.xy;
}

void main() {

   
   ret = color * texture(textures[0], re_pos_uv(vert_in.UV, uv_position_scale)) * vec4(vert_in.COLOR.x,vert_in.COLOR.y,vert_in.COLOR.z,1);
   //ret = vec4(vert_in.NORMAL_COLOR,color.w);

}