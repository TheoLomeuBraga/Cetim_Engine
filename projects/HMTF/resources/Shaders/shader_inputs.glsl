uniform bool shedow_mode;
uniform sampler2D texturas[12];
uniform sampler2D post_procesing_render_input[6];
uniform float inputs[32];
uniform vec4 color;
uniform vec4 back_ground_color;
uniform float gama,time,metallic,softness;
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