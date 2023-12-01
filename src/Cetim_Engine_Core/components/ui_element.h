#pragma once

#include <iostream>
#include <memory>
using namespace std;
#include "RecursosT.h"
#include "transform.h"
#include "ManusearArquivos.h"
#include "input.h"

#include "game_object.h"
#include "render_shader.h"
#include "render_text.h"

struct ui_style_struct{
    vec4 color;
    vec4 background_color;
    vec4 border_color;

    float border_size = 1;

    shared_ptr<imagem> background_image;
    shared_ptr<imagem> border_image;

    shared_ptr<fonte> text_font;
};
typedef struct ui_style_struct ui_style;

enum ui_type {
    lable = 0,
    text = 1,
    button = 2,
};

class ui_componente : public componente
{
public:

    static vec2 cursor_position;

    unsigned char type;

    ui_componente *father;

    vec2 position;
    vec2 scale;

    ui_style normal_style;
    ui_style hover_style;
    ui_style click_style;

    wstring text;

    shared_ptr<objeto_jogo> text_obj;
    shared_ptr<objeto_jogo> background_obj;
    shared_ptr<objeto_jogo> border_obj;

    ui_componente() {}

    void iniciar() {

        text_obj = novo_objeto_jogo();
        cena_objetos_selecionados->adicionar_objeto(esse_objeto,text_obj);

        background_obj = novo_objeto_jogo();
        cena_objetos_selecionados->adicionar_objeto(esse_objeto,background_obj);

        border_obj = novo_objeto_jogo();
        cena_objetos_selecionados->adicionar_objeto(esse_objeto,border_obj);

    }

	void atualisar() {}

	void finalisar() {
        text_obj->excluir();
        background_obj->excluir();
        border_obj->excluir();
    }

    ~ui_componente() {}
};

vec2 ui_componente::cursor_position = vec2(0,0);
