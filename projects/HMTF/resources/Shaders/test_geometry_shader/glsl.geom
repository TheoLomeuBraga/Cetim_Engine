#version 330 core
#extension GL_ARB_separate_shader_objects : require
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex {
    vec4 POS;
    vec2 UV;
} vert_out[];

out Vertex {
    vec4 POS;
    vec2 UV;
} geom_out;

uniform bool ui;
uniform mat4 projection, vision, transform;

void pass_vertex(int i) {
    geom_out.POS = vert_out[i].POS;
    geom_out.UV = vert_out[i].UV;
    if(ui) {
        gl_Position = transform * vert_out[i].POS;
    } else {
        gl_Position = (projection * vision * transform) * vert_out[i].POS;
    }

    //psx factor
   int psx_factor = 8;
   gl_Position.x = (float(int(gl_Position.x * psx_factor) + 0.5)) / psx_factor;
   gl_Position.y = (float(int(gl_Position.y * psx_factor) + 0.5)) / psx_factor;
   geom_out.POS.x = gl_Position.x
   geom_out.POS.y = gl_Position.y
   vert_out.UV.x = (float(int(vert_out.UV.x * psx_factor) + 0.5)) / psx_factor;
   vert_out.UV.y = (float(int(vert_out.UV.y * psx_factor) + 0.5)) / psx_factor;

    EmitVertex();
}

void main() {
    pass_vertex(0);
    pass_vertex(1);
    pass_vertex(2);
    EndPrimitive();
}
