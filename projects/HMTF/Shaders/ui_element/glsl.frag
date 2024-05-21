#version 330 core

in vec4 pos;
in vec2 uv;
layout(location = 0) out vec4 ret;


uniform float roundnes;
uniform float border_size;
uniform vec4 color;
uniform vec4 border_color;
uniform sampler2D image;
uniform sampler2D border_image;


float is_border() {
    float bs = border_size / 2.0;
    float corner_radius = roundnes;
    if(roundnes == 0.0){
        corner_radius = 0.005;
    }
    float inner_width = 1.0 - 2.0 * corner_radius;
    float inner_height = 1.0 - 2.0 * corner_radius;

    // Verifica se está nas bordas laterais retas
    if (uv.x < bs + corner_radius || uv.x > 1.0 - bs - corner_radius || uv.y < bs + corner_radius || uv.y > 1.0 - bs - corner_radius) {
        return 1.0f; // Está na borda
    }
    
    // Verifica se está nas áreas arredondadas dos cantos
    vec2 uv_centered = uv - 0.5; // Centraliza as coordenadas uv
    uv_centered.x *= 2.0; // Escala para -1.0 a 1.0
    uv_centered.y *= 2.0; // Escala para -1.0 a 1.0
    
    // Calcula a distância do centro do canto mais próximo
    float dist = length(uv_centered - vec2(clamp(uv_centered.x, -inner_width, inner_width), clamp(uv_centered.y, -inner_height, inner_height)));
    if (dist < corner_radius + bs && dist > corner_radius - bs) {
        return 1.0f;
    }
    
    return 0.0f; // Não está na borda
}


float is_valid_spot() {
    if(roundnes == 0.0){return 1.0;}
    float corner_radius = roundnes;
    float inner_width = 1.0 - 2.0 * corner_radius;
    float inner_height = 1.0 - 2.0 * corner_radius;

    vec2 uv_centered = uv - 0.5;
    uv_centered.x *= 2.0;
    uv_centered.y *= 2.0;

    // Distância do ponto ao centro do canto mais próximo
    float dist = length(uv_centered - vec2(clamp(uv_centered.x, -inner_width, inner_width), clamp(uv_centered.y, -inner_height, inner_height)));
    if (dist < corner_radius) {
        return 1.0f; // Ponto válido, está dentro do raio do canto arredondado
    }

    return 0.0f; // Ponto não válido
}

//fun�oes
vec2 re_pos_uv(vec2 UV, vec4 UV_PosSca) {
    return uv * UV_PosSca.zw + UV_PosSca.xy;
}

void main() {

    
    vec4 ib = mix(color * texture2D(image,uv),border_color * texture2D(border_image,uv),is_border());
    ret = vec4(ib.xyz,ib.w * is_valid_spot());
    

}