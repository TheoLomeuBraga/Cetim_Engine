#version 330 core
#extension GL_ARB_separate_shader_objects : require
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 ret;

// material
uniform sampler2D post_procesing_render_input[6];
uniform sampler2D textures[6];
uniform vec4 color;
uniform float gama;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
    return UV * UV_PosSca.zw + UV_PosSca.xy;
}

vec4 ditheredTextureColor(sampler2D tex, vec2 uv, float threshold) {
    vec4 texColor = texture(tex, uv);
    
    // Coordenadas de dithering
    ivec2 ditherCoords = ivec2(gl_FragCoord.xy) % 4;

    // 1D dither matrix (flattened from the 4x4 matrix)
    int ditherMatrix[16] = int[16](
        0, 8, 2, 10,
        12, 4, 14, 6,
        3, 11, 1, 9,
        15, 7, 13, 5
    );

    // Calculate the index for the 1D dither matrix
    int ditherIndex = ditherCoords.x + ditherCoords.y * 4;

    // Adjust the fragment color based on the dithering value
    float ditherValue = float(ditherMatrix[ditherIndex]) / 15.0;
    if (ditherValue < threshold) {
        return vec4(0.0); // Black color
    } else {
        return texColor;
    }
}

void main() {

    ret = color * texture(post_procesing_render_input[0], uv);
    //ret = color * ditheredTextureColor(post_procesing_render_input[0], uv,0.001);

    //limit color pallet
    const float numBits = 4.0;
    ret = floor(ret * (pow(2.0, numBits) - 1.0)) / (pow(2.0, numBits) - 1.0);

    //add ditering

}