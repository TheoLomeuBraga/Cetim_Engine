uniform bool shedow_mode;
uniform sampler2D texturas[6];
uniform sampler2D post_procesing_render_input[6];
uniform float inputs[16];
uniform vec4 color;
uniform float gama,time;
uniform bool ui;
uniform mat4 projection,vision,transform;
uniform vec4 uv_position_scale;

struct Light {
  int type;
  vec3 position;
  vec3 direction;
  vec3 color;
  float streight;
  float angle;
  
  
} light[99];

layout(location = 0) in struct Vertex
{
  vec3 position;
  vec2 uv;
  vec3 normal;
  vec3 color;
} vertex;


//https://stackoverflow.com/questions/19081734/passing-custom-vertex-structure-to-glsl-shaders
layout(location = 0) in struct Vertex
{
  vec3 position;
  vec2 uv;
  vec3 normal;
  vec3 color;
} vertex;

vec3 quad_data[4] = vec3[4](
  vec3(1,-1,0),
  vec3(1,1,0),
  vec3(-1,-1,0),
  vec3(-1,1,0)
  );
  
vec3 quad_data[6] = vec3[6](
  vec3(-1,-1,0),
  vec3(1,-1,0),
  vec3(1,1,0),
  
  vec3(1,1,0),
  vec3(-1,1,0),
  vec3(-1,-1,0)
  );