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