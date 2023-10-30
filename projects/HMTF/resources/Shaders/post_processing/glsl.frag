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

// Função que aplica o dithering estilo PlayStation 1 a uma textura com ajuste de intensidade
vec4 applyPSXDithering(sampler2D texture, vec2 uv, float intensity) {
    // Matriz de padrões de dithering
    const mat3 ditherMatrix = mat3(
        vec3( 1.0, 5.0, 3.0),
        vec3( 7.0, 9.0, 6.0),
        vec3( 4.0, 2.0, 8.0)
    );

    // Tamanho da textura
    vec2 textureSize = textureSize(texture, 0);

    // Coordenada de textura ajustada
    vec2 adjustedUV = uv * textureSize - 0.5;

    // Índices inteiros para selecionar o padrão de dithering
    ivec2 ditherIndex = ivec2(adjustedUV) % 3;

    // Padrão de dithering na posição do pixel
    float ditherValue = ditherMatrix[ditherIndex.x][ditherIndex.y] / 9.0;

    // Ajuste da intensidade do dithering
    ditherValue *= intensity;

    // Coordenada de textura ajustada com o dithering
    vec2 ditheredUV = (adjustedUV + ditherValue) / textureSize;

    // Obtém a cor da textura na coordenada ditheredUV
    vec4 color = texture2D(texture, ditheredUV);

    const float numBits = 4.0;
    color = floor(color * (pow(2.0, numBits) - 1.0)) / (pow(2.0, numBits) - 1.0);

    return color;
}


void main() {

    //ret = color * texture(post_procesing_render_input[0], uv);

    //adicionar Dithering
    ret = color * applyPSXDithering(post_procesing_render_input[0], uv,0.2);

    

}