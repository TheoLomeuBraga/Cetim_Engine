vec3 rgbToHsl(vec3 color) {
    float r = color.r;
    float g = color.g;
    float b = color.b;

    float max = max(max(r, g), b);
    float min = min(min(r, g), b);
    float h, s, l = (max + min) / 2.0;

    if (max == min) {
        h = s = 0.0; // Achromatic
    } else {
        float d = max - min;
        s = l > 0.5 ? d / (2.0 - max - min) : d / (max + min);
        if (max == r) {
            h = (g - b) / d + (g < b ? 6.0 : 0.0);
        } else if (max == g) {
            h = (b - r) / d + 2.0;
        } else if (max == b) {
            h = (r - g) / d + 4.0;
        }
        h /= 6.0;
    }

    return vec3(h, s, l);
}

float hueToRgb(float p, float q, float t) {
    if (t < 0.0) t += 1.0;
    if (t > 1.0) t -= 1.0;
    if (t < 1.0/6.0) return p + (q - p) * 6.0 * t;
    if (t < 1.0/2.0) return q;
    if (t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
    return p;
}

vec3 hslToRgb(vec3 hsl) {
    float h = hsl.x;
    float s = hsl.y;
    float l = hsl.z;

    vec3 rgb;

    if (s == 0.0) {
        rgb = vec3(l); // Achromatic
    } else {
        float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        float p = 2.0 * l - q;
        rgb.r = hueToRgb(p, q, h + 1.0/3.0);
        rgb.g = hueToRgb(p, q, h);
        rgb.b = hueToRgb(p, q, h - 1.0/3.0);
    }

    return rgb;
}


// Função que aplica o dithering estilo PlayStation 1 a uma textura com ajuste de intensidade
const float dither[16] = float[](1.0 / 17.0, 9.0 / 17.0, 3.0 / 17.0, 11.0 / 17.0, 13.0 / 17.0, 5.0 / 17.0, 15.0 / 17.0, 7.0 / 17.0, 4.0 / 17.0, 12.0 / 17.0, 2.0 / 17.0, 10.0 / 17.0, 16.0 / 17.0, 8.0 / 17.0, 14.0 / 17.0, 6.0 / 17.0);
vec4 Dithering(sampler2D texture, vec2 uv, float power) {
    ivec2 textureSize = textureSize(texture, 0) * 2;
    // Padrão de dithering 4x4 (Bayer matrix)

    // Ajusta o padrão de dithering com base na resolução da textura
    float ditherScale = 4.0; // você pode ajustar este valor para mais ou menos granularidade
    vec2 scaledUV = uv * vec2(textureSize) / ditherScale;

    // Calcula a posição no padrão de dithering
    vec2 ditherPos = mod(floor(scaledUV), vec2(4.0, 4.0));
    int index = int(ditherPos.y * 4.0 + ditherPos.x);

    // Obtém a cor original da textura
    vec4 color = texture2D(texture, uv);

    // Aplica o padrão de dithering
    float ditherValue = dither[index];
    float ditherIntensity = (ditherValue - 0.5) * 0.04 * power; // Modula a intensidade do dithering com 'power'
    vec3 ditheredColor = color.rgb + ditherIntensity;

    return vec4(ditheredColor, color.a);
}

vec4 generateWhiteBorder(sampler2D texture, vec2 uv,float threshold) {
    // Define os offsets para detectar as bordas
    float scale = 1.0 / 128.0; // Reduzir a escala aumenta a detecção de bordas
    vec2 offsets[8] = vec2[](vec2(-scale, 0.0), vec2(scale, 0.0), vec2(0.0, -scale), vec2(0.0, scale),
                             vec2(-scale, -scale), vec2(-scale, scale), vec2(scale, -scale), vec2(scale, scale));

    // Calcula o gradiente da cor
    vec3 centerColor = texture2D(texture, uv).rgb;
    float maxDiff = 0.0;
    for (int i = 0; i < 8; i++) {
        vec3 neighborColor = texture2D(texture, uv + offsets[i]).rgb;
        maxDiff = max(maxDiff, length(centerColor - neighborColor));
    }

    // Se o gradiente for maior que o limiar, pinte de branco, senão de preto
    return maxDiff > threshold ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 applyXBRFilter(sampler2D tex, vec2 uv) {
    ivec2 texSize = textureSize(tex, 0);
    ivec2 uvInt = ivec2(uv * texSize);
    vec2 texOffset = 1.0 / vec2(texSize);

    // Coeficientes para o filtro XBR
    float coef[12] = float[12](1.0 / 256.0, 2.0 / 256.0, 1.0 / 256.0, 2.0 / 256.0, 4.0 / 256.0, 2.0 / 256.0, 1.0 / 256.0, 2.0 / 256.0, 1.0 / 256.0, 2.0 / 256.0, 4.0 / 256.0, 2.0 / 256.0);

    vec4 result = vec4(0.0);

    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
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
    if(bits < 1 || bits > 7) {
        return color; // Retorna a cor original se bits não está no intervalo [1, 7]
    }

    float maxValue = pow(2.0, float(bits)) - 1.0; // Calcula o valor máximo baseado no número de bits
    vec4 scaledColor = color * maxValue; // Escala a cor para o intervalo do número de bits
    vec4 reducedColor = floor(scaledColor + 0.5) / maxValue; // Reduz os bits e normaliza

    return reducedColor;
}