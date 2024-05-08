#version 330 core
#extension GL_ARB_separate_shader_objects : require
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 ret;

// material
 uniform sampler2D char_texture;
uniform vec4 color;

//fun�oes
vec2 re_pos_uv(vec2 UV,vec4 UV_PosSca){
return UV * UV_PosSca.zw + UV_PosSca.xy;
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

void main(){
        
    float f = applyXBRFilter(char_texture,uv).x;
    if(f > 0.01){
        f = 1;
    }else{
        f = 0;
    }
    ret = vec4(color.x,color.y,color.z,f);
  
}