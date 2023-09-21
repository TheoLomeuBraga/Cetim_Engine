#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex {
    vec4 POS;
    vec2 UV;
} vert_in[];

out Vertex {
    vec4 POS;
    vec2 UV;
} vert_out;

uniform float primitives_size;

void main() {
    
    vec3 center = (vert_in[0].POS.xyz + vert_in[1].POS.xyz + vert_in[2].POS.xyz) / 3.0;

    for (int i = 0; i < 3; ++i) {
        vec3 new_pos = mix(center,vert_in[i].POS.xyz,primitives_size);
        //vert_out.POS = vec4(new_pos,1);

        vert_out.POS = vert_in[i].POS;
        vert_out.UV = vert_in[i].UV;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
