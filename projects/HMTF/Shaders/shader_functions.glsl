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

//ditter

const vec3 _Colors = vec3(32.0);

float channelError(float col, float colMin, float colMax) {
    float range = abs(colMin - colMax);
    float aRange = abs(col - colMin);
    return aRange / range;
}

vec3 applyDithering(vec2 uv) {
    // Define a matriz de dithering 8x8 (Bayer matrix)
    float bayerMatrix[64] = float[](0, 48, 12, 60, 3, 51, 15, 63, 32, 16, 44, 28, 35, 19, 47, 31, 8, 56, 4, 52, 11, 59, 7, 55, 40, 24, 36, 20, 43, 27, 39, 23, 2, 50, 14, 62, 1, 49, 13, 61, 34, 18, 46, 30, 33, 17, 45, 29, 10, 58, 6, 54, 9, 57, 5, 53, 42, 26, 38, 22, 41, 25, 37, 21);

    // Escala as coordenadas UV para o intervalo da matriz 8x8
    vec2 scaledUV = mod(floor(uv * 8.0), 8.0);
    int index = int(scaledUV.y * 8.0 + scaledUV.x);

    // Normaliza o valor da matriz para [0, 1]
    float ditherValue = bayerMatrix[index] / 63.0;

    // Retorna o valor de dithering como uma cor cinza
    return vec3(ditherValue);
}

float ditheredChannel(float error, vec2 ditherBlockUV) {
    float pattern = applyDithering(ditherBlockUV).r;
    if(error > pattern) {
        return 1.0;
    } else {
        return 0.0;
    }
}

vec4 mix(vec4 a, vec4 b, float amt) {
    return ((1.0 - amt) * a) + (b * amt);
}

/// YUV/RGB color space calculations

vec3 RGBtoYUV(vec3 rgb) {
    vec3 yuv;
    yuv.r = rgb.r * 0.2126 + 0.7152 * rgb.g + 0.0722 * rgb.b;
    yuv.g = (rgb.b - yuv.r) / 1.8556;
    yuv.b = (rgb.r - yuv.r) / 1.5748;

    // Adjust to work on GPU
    yuv.gb += 0.5;

    return yuv;
}

vec3 YUVtoRGB(vec3 yuv) {
    yuv.gb -= 0.5;
    return vec3(yuv.r * 1.0 + yuv.g * 0.0 + yuv.b * 1.5748, yuv.r * 1.0 + yuv.g * -0.187324 + yuv.b * -0.468124, yuv.r * 1.0 + yuv.g * 1.8556 + yuv.b * 0.0);
}

float fmix(float a, float b, float t) {
    return a * (1.0 - t) + b * t;
}

vec3 ditherColor(vec3 col, vec2 uv, float xres, float yres) {
    vec3 yuv = RGBtoYUV(col);

    vec3 col1 = floor(yuv * _Colors) / _Colors;
    vec3 col2 = ceil(yuv * _Colors) / _Colors;

    // Calculate dither texture UV based on the input texture
    vec2 ditherBlockUV = uv * vec2(xres / 8.0, yres / 8.0);

    yuv.x = fmix(col1.x, col2.x, ditheredChannel(channelError(yuv.x, col1.x, col2.x), ditherBlockUV));
    yuv.y = fmix(col1.y, col2.y, ditheredChannel(channelError(yuv.y, col1.y, col2.y), ditherBlockUV));
    yuv.z = fmix(col1.z, col2.z, ditheredChannel(channelError(yuv.z, col1.z, col2.z), ditherBlockUV));

    return (YUVtoRGB(yuv));
}

vec3 getPixel(vec2 uv) {
    return texture(post_procesing_render_input[0], uv).rgb;
}