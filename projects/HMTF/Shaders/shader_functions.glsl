// Função que aplica o dithering estilo PlayStation 1 a uma textura com ajuste de intensidade
vec4 applyPSXDithering(sampler2D texture, vec2 uv, float intensity,float palet_limitation_intensity) {

    vec2 textureSize2d = vec2(1,1) / textureSize(post_procesing_render_input[0],0) / 2;
    //uv = vec2(uv.x + textureSize2d.x,uv.y + textureSize2d.y);

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

    color = floor(color * (pow(2.0, palet_limitation_intensity) - 1.0)) / (pow(2.0, palet_limitation_intensity) - 1.0);

    return color;
}

vec4 applyXBRFilter(sampler2D tex, vec2 uv)
{
    ivec2 texSize = textureSize(tex, 0);
    ivec2 uvInt = ivec2(uv * texSize);
    vec2 texOffset = 1.0 / vec2(texSize);

    // Coeficientes para o filtro XBR
    float coef[12] = float[12](
        1.0 / 256.0, 2.0 / 256.0, 1.0 / 256.0,
        2.0 / 256.0, 4.0 / 256.0, 2.0 / 256.0,
        1.0 / 256.0, 2.0 / 256.0, 1.0 / 256.0,
        2.0 / 256.0, 4.0 / 256.0, 2.0 / 256.0
    );

    vec4 result = vec4(0.0);

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            ivec2 offset = ivec2(i, j);
            vec2 sampleUV = (uvInt + offset) * texOffset;
            vec4 sampleColor = texture(tex, sampleUV);

            // Ponderação do filtro XBR
            int coefIndex = (i + 1) * 3 + (j + 1);
            result += coef[coefIndex] * sampleColor;
        }
    }

    return result;
}

vec4 reduce_color_bits(vec4 color, int bits) {
    if (bits < 1 || bits > 7) {
        return color; // Retorna a cor original se bits não está no intervalo [1, 7]
    }

    float maxValue = pow(2.0, float(bits)) - 1.0; // Calcula o valor máximo baseado no número de bits
    vec4 scaledColor = color * maxValue; // Escala a cor para o intervalo do número de bits
    vec4 reducedColor = floor(scaledColor + 0.5) / maxValue; // Reduz os bits e normaliza

    return reducedColor;
}