vec4 psxVertexPrecision(vec4 vertexPosition) {
   float precision = 256.0; // Change this to control the level of precision
   vec3 quantizedPosition = round(vertexPosition * precision) / precision;
   return vec4(quantizedPosition,vertexPosition.w);
}

vec3 psxVertexPrecision(vec3 vertexPosition, vec2 uv) {
   const float precision = 256.0; // Change this to control the level of precision
   vec3 quantizedPosition = round(vertexPosition * precision) / precision;
   return quantizedPosition;
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