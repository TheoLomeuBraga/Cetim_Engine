#pragma once

#include <iostream>
#include <memory>
using namespace std;
#include "RecursosT.h"



#include "transform.h"
#include "render_shader.h"
#include "render_text.h"

#include "LoopPrincipal.h"


struct ui_style_struct{
    vec4 color = vec4(1,0,0,1);
    vec4 background_color = vec4(0,1,0,1);
    vec4 border_color = vec4(0,0,1,1);

    float border_size = 0.1;

    shared_ptr<imagem> background_image = NULL;
    shared_ptr<imagem> border_image = NULL;

    shared_ptr<fonte> text_font = NULL;
};
typedef struct ui_style_struct ui_style;

enum ui_type {
    lable = 0,
    text = 1,
    button = 2,
};

class ui_componente : public componente
{
    shared_ptr<transform_> tf;
    shared_ptr<objeto_jogo> text_obj,background_obj,border_obj;
public:

    static vec2 cursor_position;
    uint8_t camada = 0;
    bool ligado = true;
    ui_type type;
    render_text_location text_location_x,text_location_y;
    shared_ptr<ui_componente> father;
    vec2 global_position = vec2(0,0), position = vec2(0.25,0.25),scale = vec2(0.5,0.5);
    ui_style normal_style,hover_style,click_style;
    wstring text;

    ui_componente() {}

    void iniciar() {

        esse_objeto->adicionar_componente<transform_>(transform_());
        tf = esse_objeto->pegar_componente<transform_>();
        tf->UI = true;

        text_obj = novo_objeto_jogo();
        text_obj->adicionar_componente<transform_>(transform_());
        cena_objetos_selecionados->adicionar_objeto(esse_objeto,text_obj);

        background_obj = novo_objeto_jogo();
        background_obj->adicionar_componente<transform_>(transform_());
        cena_objetos_selecionados->adicionar_objeto(esse_objeto,background_obj);

        border_obj = novo_objeto_jogo();
        border_obj->adicionar_componente<transform_>(transform_());
        cena_objetos_selecionados->adicionar_objeto(esse_objeto,border_obj);

    }

	void atualisar() {

        if(esse_objeto->pai != NULL && esse_objeto->pai->pegar_componente<ui_componente>() != NULL){
            father = esse_objeto->pai->pegar_componente<ui_componente>();
            global_position += father.global_position + global_position
        }else{
            father = NULL;
            global_position = position;
        }

        

        

    }

	void finalisar() {
        text_obj->excluir();
        background_obj->excluir();
        border_obj->excluir();
    }

    ~ui_componente() {}
};

vec2 ui_componente::cursor_position = vec2(0,0);


void test_ui(){
    shared_ptr<objeto_jogo> test_obj =  novo_objeto_jogo();
    test_obj->adicionar_componente<ui_componente>(ui_componente());
    shared_ptr<ui_componente> uic = test_obj->pegar_componente<ui_componente>();

    ui_style style;
    style.text_font = ManuseioDados::carregar_fonte("resources/Fonts/Glowworm Regular.json");
    uic->normal_style = style;
    uic->hover_style = style;
    uic->click_style = style;
    
    cena_objetos_selecionados->adicionar_objeto(test_obj);
}