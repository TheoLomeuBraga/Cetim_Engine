#version 300 es
precision lowp int;
precision lowp float;

in vec4 pos;
in vec2 uv;

layout(location = 0) out vec4 ret;

// material
uniform sampler2D post_procesing_render_input[16];
uniform sampler2D textures[6];
uniform vec4 color;
uniform float gama;

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
    return UV * UV_PosSca.zw + UV_PosSca.xy;
}

vec4 reduce_color_bits(vec4 color, float bits) {
    if(bits < 1.0 || bits > 7.0) {
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
    float bayerMatrix[64] = float[](0.0, 48.0, 12.0, 60.0, 3.0, 51.0, 15.0, 63.0, 32.0, 16.0, 44.0, 28.0, 35.0, 19.0, 47.0, 31.0, 8.0, 56.0, 4.0, 52.0, 11.0, 59.0, 7.0, 55.0, 40.0, 24.0, 36.0, 20.0, 43.0, 27.0, 39.0, 23.0, 2.0, 50.0, 14.0, 62.0, 1.0, 49.0, 13.0, 61.0, 34.0, 18.0, 46.0, 30.0, 33.0, 17.0, 45.0, 29.0, 10.0, 58.0, 6.0, 54.0, 9.0, 57.0, 5.0, 53.0, 42.0, 26.0, 38.0, 22.0, 41.0, 25.0, 37.0, 21.0);

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

//László Matuska is my hero

void main() {

    const float ditter = 1.0;

    if(ditter > 0.0) {
         ret = color * reduce_color_bits(texture(post_procesing_render_input[0], uv, 0.01), 4.0);

    // Normalized pixel coordinates (from 0 to 1)
        vec2 tex_size = vec2(textureSize(post_procesing_render_input[0], 0));
        vec2 uv = (uv * tex_size) / tex_size;

    // Time varying pixel color
        vec3 col = getPixel(uv);
        col = ditherColor(col, uv, tex_size.x, tex_size.y);
    // Output to screen
        ret = vec4(col, 1.0);
    } else {
        ret = color * reduce_color_bits(texture(post_procesing_render_input[0], uv, 0.01), 4.0);
    }
}