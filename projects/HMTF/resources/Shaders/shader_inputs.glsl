layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;
layout(location = 3) in ivec4 boneIds; 
layout(location = 4) in vec4 weights;

uniform bool shedow_mode;
uniform bool skin_mode;
uniform sampler2D texturas[12];
uniform sampler2D post_procesing_render_input[6];
uniform float inputs[32];
uniform vec4 color;
uniform float gama,time,metallic,softness;
uniform bool ui;
uniform mat4 projection,vision,transform;
uniform vec4 uv_position_scale;

const int MAX_BONES = 256;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

vec2 position = vec2(0, 0);
vec2 ui_size = vec2(0.1, 0.1);
vec4 back_ground_color = vec4(1, 1, 1, 1);
vec4 color_hover = vec4(0, 0, 0, 1);
vec4 back_ground_color_hover = vec4(1, 1, 1, 1);
vec4 color_click = vec4(0, 0, 0, 1);
vec4 back_ground_color_click = vec4(1, 1, 1, 1);

struct Light {
  int type;
  vec3 position;
  vec3 direction;
  vec3 color;
  float intensity;
  float angle;  
} ;

uniform Light light[99];

light_size

layout(location = 0) in struct Vertex
{
  vec3 position;
  vec2 uv;
  vec3 normal;
  vec3 color;
} vertex_in;

layout(location = 0) out struct Vertex
{
  vec3 position;
  vec2 uv;
  vec3 normal;
  vec3 color;
} vertex_out;

vec3 quad_data[4] = vec3[4](
  vec3(1,-1,0),
  vec3(1,1,0),
  vec3(-1,-1,0),
  vec3(-1,1,0)
  );
  
vec3 center_box_data[6] = vec3[6](
  vec3(-1,-1,0),
  vec3(1,-1,0),
  vec3(1,1,0),
  
  vec3(1,1,0),
  vec3(-1,1,0),
  vec3(-1,-1,0)
  );

  vec3 top_side_box_data[6] = vec3[6](
  vec3(0,0,0),
  vec3(1,0,0),
  vec3(1,1,0),
  
  vec3(1,1,0),
  vec3(0,1,0),
  vec3(0,0,0)
  );