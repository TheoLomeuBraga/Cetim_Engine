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

#include "table.h"

struct ui_style_struct
{
    vec4 text_color = vec4(1, 0, 0, 1);
    vec4 background_color = vec4(0, 1, 0, 1);
    vec4 border_color = vec4(0, 0, 1, 1);

    float text_size = 0.1, border_size = 0.05, space_betwen_lines = 2;
    bool uniform_spaces_betwen_chars = false;

    shared_ptr<imagem> background_image = NULL;
    shared_ptr<imagem> border_image = NULL;

    shared_ptr<fonte> text_font = NULL;
};
typedef struct ui_style_struct ui_style;

enum ui_type
{
    common = 0,
    check_box = 1,
    slider = 2,
};

void function_reference_example(string state, string id)
{
    // print({id,state});
}

class ui_componente : public componente
{
    shared_ptr<transform_> tf;
    shared_ptr<objeto_jogo> base_obj, text_obj, background_obj, border_obj;

    vec2 base_position = vec2(0, 0);

    
    ui_style current_style;

public:
    static vec2 cursor_position;
    static bool click,click_last_frame,first_click_frame;
    unsigned char ui_type = common;
    string id, data;
    uint8_t render_layer = 4;
    float space_betwen_lines = 2;
    bool uniform_spaces_betwen_chars = false;
    unsigned char text_location_x = render_text_location::RIGHT, text_location_y = render_text_location::CENTER;
    shared_ptr<ui_componente> father;
    vec2 position = vec2(0.0, 0.0), scale = vec2(0.2, 0.2);
    ui_style normal_style, hover_style, click_style;
    wstring text;
    string state = "none";

    void (*function_reference)(string id, string state) = function_reference_example;
    LuaFunctionWrapper lua_function = {NULL, ""};

    void call_lua_function(string state, string id)
    {
        if (lua_function.L != NULL)
        {
            lua_getglobal(lua_function.L, lua_function.functionRef.c_str());

            lua_pushstring(lua_function.L, id.c_str());
            lua_pushstring(lua_function.L, state.c_str());
            lua_pushstring(lua_function.L, data.c_str());

            lua_pcall(lua_function.L, 3, 0, 0);
        }
    }

    ui_componente() {}

    void iniciar()
    {

        // esse_objeto->adicionar_componente<transform_>(transform_());
        // tf = esse_objeto->pegar_componente<transform_>();
        // tf->UI = true;

        Material mat;
        mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        mat.shad = "resources/Shaders/ui_componente";

        base_obj = novo_objeto_jogo();
        cena_objetos_selecionados->adicionar_objeto(esse_objeto, base_obj);

        mat.cor = vec4(1, 0, 0, 1);
        border_obj = novo_objeto_jogo();
        border_obj->adicionar_componente<transform_>(transform_());
        border_obj->pegar_componente<transform_>()->UI = true;
        border_obj->pegar_componente<transform_>()->usar_pai = false;
        border_obj->adicionar_componente<render_shader>(render_shader());
        border_obj->pegar_componente<render_shader>()->mat = mat;
        cena_objetos_selecionados->adicionar_objeto(base_obj, border_obj);

        mat.cor = vec4(0, 1, 0, 1);
        background_obj = novo_objeto_jogo();
        background_obj->adicionar_componente<transform_>(transform_());
        background_obj->pegar_componente<transform_>()->UI = true;
        background_obj->pegar_componente<transform_>()->usar_pai = false;
        background_obj->adicionar_componente<render_shader>(render_shader());
        background_obj->pegar_componente<render_shader>()->mat = mat;
        cena_objetos_selecionados->adicionar_objeto(base_obj, background_obj);

        mat.cor = vec4(0, 0, 1, 1);
        mat.shad = "resources/Shaders/text";
        text_obj = novo_objeto_jogo();
        text_obj->adicionar_componente<transform_>(transform_());
        text_obj->pegar_componente<transform_>()->UI = true;
        text_obj->pegar_componente<transform_>()->usar_pai = false;
        text_obj->adicionar_componente<render_texto>(render_texto());
        text_obj->pegar_componente<render_texto>()->mat = mat;

        cena_objetos_selecionados->adicionar_objeto(base_obj, text_obj);
    }

    bool is_above()
    {
        vec2 acurate_pos = vec2(position.x + base_position.x, position.y + base_position.y);
        acurate_pos += vec2(-scale.x / 2, scale.y / 2);

        if (ui_componente::cursor_position.x > acurate_pos.x && ui_componente::cursor_position.x < acurate_pos.x + scale.x)
        {
            float new_cursor_position_y = (-ui_componente::cursor_position.y + 1);
            if (new_cursor_position_y < acurate_pos.y && new_cursor_position_y > acurate_pos.y - scale.y)
            {
                return true;
            }
        }
        return false;
    }

    void atualisar()
    {

        base_position = vec2(0, 0);
        /**/
        father = esse_objeto->pai->pegar_componente<ui_componente>();

        if (esse_objeto->pai != NULL && father != NULL)
        {
            base_position = father->position + father->base_position;
            // print({"father->base_position",father->base_position.x,father->base_position.y});
        }
        else
        {
            father = NULL;
        }

        /*

        if (is_above())
        {
            if (click)
            {
                current_style = click_style;
                if (!first_click_frame)
                {
                    function_reference(id, "click");
                    call_lua_function(id, "click");
                    state = "click";
                }
                else
                {
                    function_reference(id, "hold");
                    call_lua_function(id, "hold");
                    state = "hold";
                }
                first_click_frame = true;

            }
            else
            {
                current_style = hover_style;
                function_reference(id, "hover");
                call_lua_function(id, "hover");
                first_click_frame = false;
                state = "hover";
            }

        }
        else
        {
            current_style = normal_style;
            first_click_frame = false;
            state = "none";
        }

        */

       if (is_above() && lua_function.L != NULL)
        {
            if (click)
            {
                current_style = click_style;
                if (!click_last_frame)
                {
                    function_reference(id, "click");
                    call_lua_function(id, "click");
                    state = "click";
                    click_last_frame = true;
                }
                else
                {
                    function_reference(id, "hold");
                    call_lua_function(id, "hold");
                    state = "hold";
                }
                

            }
            else
            {
                current_style = hover_style;
                function_reference(id, "hover");
                call_lua_function(id, "hover");
                state = "hover";
            }

        }
        else
        {
            current_style = normal_style;
            state = "none";
        }

        

        vec2 acurate_pos = vec2(((position.x + base_position.x) * 2) - 1, ((position.y + base_position.y) * 2) - 1);

        text_obj->pegar_componente<transform_>()->pos = vec3(acurate_pos.x, acurate_pos.y, 0);
        text_obj->pegar_componente<transform_>()->esca = vec3(current_style.text_size, current_style.text_size, 1);
        text_obj->pegar_componente<transform_>()->mudar_angulo_graus(vec3(0, 0, 0));

        if (text_location_x == render_text_location::LEFT)
        {
            text_obj->pegar_componente<transform_>()->pos.x += scale.x;
        }
        else if (text_location_x == render_text_location::RIGHT)
        {
            text_obj->pegar_componente<transform_>()->pos.x -= scale.x;
        }

        /**/
        if (text_location_y == render_text_location::TOP)
        {
            text_obj->pegar_componente<transform_>()->pos.y += (scale.y / 2);
        }
        else if (text_location_y == render_text_location::DOWN)
        {
            text_obj->pegar_componente<transform_>()->pos.y -= (scale.y / 2);
        }

        border_obj->pegar_componente<transform_>()->pos = vec3(acurate_pos.x - (scale.x + current_style.border_size), acurate_pos.y + (scale.y + current_style.border_size), 0);
        border_obj->pegar_componente<transform_>()->esca = vec3(scale.x + current_style.border_size, scale.y + current_style.border_size, 1);
        border_obj->pegar_componente<transform_>()->mudar_angulo_graus(vec3(0, 0, 0));

        background_obj->pegar_componente<transform_>()->pos = vec3(acurate_pos.x - (scale.x), acurate_pos.y + (scale.y), 0);
        background_obj->pegar_componente<transform_>()->esca = vec3(scale.x, scale.y, 1);
        background_obj->pegar_componente<transform_>()->mudar_angulo_graus(vec3(0, 0, 0));

        Material mat;
        mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        mat.shad = "resources/Shaders/text";
        mat.cor = current_style.text_color;
        text_obj->pegar_componente<render_texto>()->mat = mat;

        mat.shad = "resources/Shaders/ui_componente";
        mat.cor = current_style.background_color;
        if (current_style.background_image != NULL)
        {
            mat.texturas[0] = current_style.background_image;
        }
        else
        {
            mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        }
        background_obj->pegar_componente<render_shader>()->mat = mat;

        mat.cor = current_style.border_color;
        if (current_style.border_image != NULL)
        {
            mat.texturas[0] = current_style.border_image;
        }
        else
        {
            mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/null.svg");
        }
        border_obj->pegar_componente<render_shader>()->mat = mat;

        border_obj->pegar_componente<render_shader>()->camada = render_layer;
        background_obj->pegar_componente<render_shader>()->camada = render_layer;
        text_obj->pegar_componente<render_texto>()->camada = render_layer;

        text_obj->pegar_componente<render_texto>()->text_location_x = text_location_x;
        text_obj->pegar_componente<render_texto>()->text_location_y = text_location_y;

        text_obj->pegar_componente<render_texto>()->uniform_space_between_characters = current_style.uniform_spaces_betwen_chars;
        text_obj->pegar_componente<render_texto>()->espaco_entre_linhas = current_style.space_betwen_lines;

        text_obj->pegar_componente<render_texto>()->texto = text;
        text_obj->pegar_componente<render_texto>()->font = ManuseioDados::carregar_fonte("resources/Fonts/Glowworm Regular.json");
    }

    void finalisar()
    {
        // text_obj->excluir();
        // background_obj->excluir();
        // border_obj->excluir();
        base_obj->excluir();
    }

    void set_text_by_string(string t)
    {
        text = convert_to_wstring(t);
    }

    ~ui_componente()
    {
    }
};
vec2 ui_componente::cursor_position = vec2(0, 0);
bool ui_componente::click_last_frame = false;
bool ui_componente::click = false;
bool ui_componente::first_click_frame = false;

void update_ui_components()
{
    ui_componente::click_last_frame = ui_componente::click;
}

void test_ui(objeto_jogo *father, LuaFunctionWrapper lw)
{

    shared_ptr<objeto_jogo> test_obj = novo_objeto_jogo();
    test_obj->adicionar_componente<ui_componente>(ui_componente());
    shared_ptr<ui_componente> uic = test_obj->pegar_componente<ui_componente>();
    uic->position = vec2(0.1, 0.6);
    uic->render_layer = 4;
    uic->id = "test_button";
    ui_style style;
    style.text_font = ManuseioDados::carregar_fonte("resources/Fonts/Glowworm Regular.json");
    uic->normal_style = style;
    style.border_color = vec4(1, 0.5, 0.5, 1);
    uic->hover_style = style;
    style.border_color = vec4(1, 0.9, 0.9, 1);
    uic->click_style = style;
    // uic->current_style = style;
    uic->text_location_x = render_text_location::LEFT;
    uic->text_location_y = render_text_location::TOP;
    uic->lua_function = lw;
    cena_objetos_selecionados->adicionar_objeto(father, test_obj);

    /**/
    shared_ptr<objeto_jogo> test_obj2 = novo_objeto_jogo();
    test_obj2->adicionar_componente<ui_componente>(ui_componente());
    shared_ptr<ui_componente> uic2 = test_obj2->pegar_componente<ui_componente>();
    uic2->render_layer = 4;
    uic2->id = "test_button";
    style = ui_style();
    style.text_font = ManuseioDados::carregar_fonte("resources/Fonts/Glowworm Regular.json");
    uic2->normal_style = style;
    style.border_color = vec4(1, 0.5, 0.5, 1);
    uic2->hover_style = style;
    style.border_color = vec4(1, 0.9, 0.9, 1);
    uic2->click_style = style;
    // uic2->current_style = style;
    uic2->position = vec2(0.2, -0.2);
    uic2->text_location_x = render_text_location::RIGHT;
    uic2->text_location_y = render_text_location::DOWN;
    uic2->lua_function = lw;
    cena_objetos_selecionados->adicionar_objeto(test_obj, test_obj2);
}