#pragma once

#include <iostream>
#include <memory>
#include <cmath>
using namespace std;
#include "RecursosT.h"

#include "transform.h"
#include "render_shader.h"
#include "render_text.h"

#include "LoopPrincipal.h"

#include "input.h"

struct ui_style_struct
{
    vec4 color = vec4(1, 0, 0, 1);
    vec4 background_color = vec4(0, 1, 0, 1);
    vec4 border_color = vec4(0, 0, 1, 1);

    float border_size = 0.05;

    shared_ptr<imagem> background_image = NULL;
    shared_ptr<imagem> border_image = NULL;

    shared_ptr<fonte> text_font = NULL;
};
typedef struct ui_style_struct ui_style;

enum ui_type
{
    lable = 0,
    text = 1,
    button = 2,
};

class ui_componente : public componente
{
    shared_ptr<transform_> tf;
    shared_ptr<objeto_jogo> text_obj, background_obj, border_obj;

public:
    static vec2 cursor_position;
    static bool click;

    uint8_t render_layer = 4;
    uint8_t camada = 0;
    bool ligado = true;
    ui_type type;
    render_text_location text_location_x, text_location_y;
    shared_ptr<ui_componente> father;
    vec2 global_position = vec2(0, 0), position = vec2(0.5, 0.5), scale = vec2(0.2, 0.2);
    ui_style normal_style, hover_style, click_style;
    wstring text;

    ui_componente() {}

    void iniciar()
    {

        // esse_objeto->adicionar_componente<transform_>(transform_());
        // tf = esse_objeto->pegar_componente<transform_>();
        // tf->UI = true;

        Material mat;
        mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        mat.shad = "resources/Shaders/ui_componente";

        mat.cor = vec4(1, 0, 0, 1);
        border_obj = novo_objeto_jogo();
        border_obj->adicionar_componente<transform_>(transform_());
        border_obj->pegar_componente<transform_>()->UI = true;
        border_obj->adicionar_componente<render_shader>(render_shader());
        border_obj->pegar_componente<render_shader>()->mat = mat;
        border_obj->pegar_componente<render_shader>()->camada = render_layer;
        cena_objetos_selecionados->adicionar_objeto(esse_objeto, border_obj);

        mat.cor = vec4(0, 1, 0, 1);
        background_obj = novo_objeto_jogo();
        background_obj->adicionar_componente<transform_>(transform_());
        background_obj->pegar_componente<transform_>()->UI = true;
        background_obj->adicionar_componente<render_shader>(render_shader());
        background_obj->pegar_componente<render_shader>()->mat = mat;
        background_obj->pegar_componente<render_shader>()->camada = render_layer;
        cena_objetos_selecionados->adicionar_objeto(esse_objeto, background_obj);

        mat.cor = vec4(0, 0, 1, 1);
        mat.shad = "resources/Shaders/text";
        text_obj = novo_objeto_jogo();
        text_obj->adicionar_componente<transform_>(transform_());
        text_obj->pegar_componente<transform_>()->UI = true;
        text_obj->adicionar_componente<render_texto>(render_texto());
        text_obj->pegar_componente<render_texto>()->mat = mat;
        text_obj->pegar_componente<render_texto>()->camada = render_layer;
        text_obj->pegar_componente<render_texto>()->texto = L"ola mundo\nx_X_x\nola mundo";
        text_obj->pegar_componente<render_texto>()->font = ManuseioDados::carregar_fonte("resources/Fonts/Glowworm Regular.json");
        cena_objetos_selecionados->adicionar_objeto(esse_objeto, text_obj);
    }

    bool is_above()
    {
        vec2 acurate_pos = vec2(mix(-1, 1, global_position.x + position.x), mix(-1, 1, global_position.y + position.y));
        if (false)
        {
            if (false)
            {
                return true;
            }
        }
        return false;
    }

    void atualisar()
    {

        /*
        if (esse_objeto->pai != NULL && esse_objeto->pai->pegar_componente<ui_componente>() != NULL)
        {
            father = esse_objeto->pai->pegar_componente<ui_componente>();
            global_position += father->global_position + global_position;
        }
        else
        {
            father = NULL;
            global_position = position;
        }
        */

        ui_style current_state = normal_style;

        

        vec2 acurate_pos = vec2(mix(-1, 1, global_position.x + position.x), mix(-1, 1, global_position.y + position.y));

        text_obj->pegar_componente<transform_>()->pos = vec3(acurate_pos.x, acurate_pos.y, 0);
        text_obj->pegar_componente<transform_>()->esca = vec3(scale.x * 0.1, scale.y * 0.1, 1);
        text_obj->pegar_componente<transform_>()->mudar_angulo_graus(vec3(0, 0, 0));

        border_obj->pegar_componente<transform_>()->pos = vec3(acurate_pos.x - (scale.x + current_state.border_size) / 2, acurate_pos.y + (scale.y + current_state.border_size) / 2, 0);
        border_obj->pegar_componente<transform_>()->esca = vec3(scale.x + current_state.border_size, scale.y + current_state.border_size, 1);
        border_obj->pegar_componente<transform_>()->mudar_angulo_graus(vec3(0, 0, 0));

        background_obj->pegar_componente<transform_>()->pos = vec3(acurate_pos.x - scale.x / 2, acurate_pos.y + scale.y / 2, 0);
        background_obj->pegar_componente<transform_>()->esca = vec3(scale.x, scale.y, 1);
        background_obj->pegar_componente<transform_>()->mudar_angulo_graus(vec3(0, 0, 0));

        Material mat;
        mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        mat.shad = "resources/Shaders/text";
        mat.cor = current_state.color;
        text_obj->pegar_componente<render_texto>()->mat = mat;

        mat.shad = "resources/Shaders/ui_componente";
        mat.cor = current_state.background_color;
        if (current_state.background_image != NULL)
        {
            mat.texturas[0] = current_state.background_image;
        }
        else
        {
            mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        }
        background_obj->pegar_componente<render_shader>()->mat = mat;

        mat.cor = current_state.border_color;
        if (current_state.border_image != NULL)
        {
            mat.texturas[0] = current_state.border_image;
        }
        else
        {
            mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        }
        border_obj->pegar_componente<render_shader>()->mat = mat;


        if(is_above()){
            print({"is above me"});
        }
    }

    void finalisar()
    {
        text_obj->excluir();
        background_obj->excluir();
        border_obj->excluir();
    }

    ~ui_componente() {}
};
vec2 ui_componente::cursor_position = vec2(0, 0);
bool ui_componente::click = false;

void update_ui_componente_test(){
    ui_componente::cursor_position.x = manuseio_inputs->mouse_input.movimentos["normalized_x"];
    ui_componente::cursor_position.y = mix(1.0,0.0,manuseio_inputs->mouse_input.movimentos["normalized_y"]);
    //print({"cursor_position",ui_componente::cursor_position.x,ui_componente::cursor_position.y});
}

void test_ui(objeto_jogo *father)
{
    shared_ptr<objeto_jogo> test_obj = novo_objeto_jogo();
    test_obj->adicionar_componente<ui_componente>(ui_componente());
    shared_ptr<ui_componente> uic = test_obj->pegar_componente<ui_componente>();
    uic->camada = 4;

    ui_style style;
    style.text_font = ManuseioDados::carregar_fonte("resources/Fonts/Glowworm Regular.json");
    uic->normal_style = style;
    uic->hover_style = style;
    uic->click_style = style;
    cena_objetos_selecionados->adicionar_objeto(father, test_obj);
}