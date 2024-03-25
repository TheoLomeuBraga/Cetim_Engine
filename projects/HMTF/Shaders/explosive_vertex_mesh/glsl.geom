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

uniform float primitives_size, elevation_distance;

uniform bool ui;
uniform mat4 projection, vision, transform;

void main() {

    vec3 center = (vert_in[0].POS.xyz + vert_in[1].POS.xyz + vert_in[2].POS.xyz) / 3.0;

    vec3 edge1 = vert_in[1].POS.xyz - vert_in[0].POS.xyz;
    vec3 edge2 = vert_in[2].POS.xyz - vert_in[0].POS.xyz;
    vec3 direction = normalize(cross(edge1, edge2));

    for(int i = 0; i < 3; ++i) {

        //ajust center distance
        vec3 new_pos = mix(vert_in[i].POS.xyz, center, primitives_size);

        new_pos += direction * elevation_distance;

        vert_out.POS = vec4(new_pos, 1);
        vert_out.UV = vert_in[i].UV;
        gl_Position = vert_out.POS;
        if(ui) {
            gl_Position = transform * vert_out.POS;
        } else {
            gl_Position = (projection * vision * transform) * vert_out.POS;
        }

        //psx factor
        int psx_factor = 8;
        gl_Position.x = (float(int(gl_Position.x * psx_factor) + 0.5)) / psx_factor;
        gl_Position.y = (float(int(gl_Position.y * psx_factor) + 0.5)) / psx_factor;
        vert_out.UV.x = (float(int(vert_out.UV.x * psx_factor) + 0.5)) / psx_factor;
        vert_out.UV.y = (float(int(vert_out.UV.y * psx_factor) + 0.5)) / psx_factor;

        EmitVertex();
    }
    EndPrimitive();
}
