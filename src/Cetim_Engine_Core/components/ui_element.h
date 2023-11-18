#pragma once

#include <iostream>
#include <memory>
using namespace std;
#include "RecursosT.h"
#include "game_object.h"
#include "transform.h"
#include "ManusearArquivos.h"
#include "input.h"

enum ui_type
{
    none = 0,
    button = 1,
    input_text = 2,
    input_number = 3,
};

enum nouse_states
{
    out = 0,
    hover = 1,
    click = 2,
};

class ui_element : public componente
{
public:
    // style
    vec2 position = vec2(0, 0);
    vec2 size = vec2(0.1, 0.1);

    vec4 color = vec4(0, 0, 0, 1);
    vec4 back_ground_color = vec4(1, 1, 1, 1);

    vec4 color_hover = vec4(0, 0, 0, 1);
    vec4 back_ground_color_hover = vec4(1, 1, 1, 1);

    vec4 color_click = vec4(0, 0, 0, 1);
    vec4 back_ground_color_click = vec4(1, 1, 1, 1);

    shared_ptr<imagem> image = NULL;
    shared_ptr<imagem> background_image = NULL;

    unsigned char type = 0;
    unsigned char mouse_state = 0;
    string function_lua_id = "";
    void(function_button)(void);

    string input_text = "";
    float input_number = 0;

    void(function_input_text)(string);
    void(function_input_number)(float);

    ui_element() {}

    void atualisar()
    {

        if (type > 0)
        {

            float x = manuseio_inputs->mouse_input.movimentos["normalized_x"];
            float y = manuseio_inputs->mouse_input.movimentos["normalized_y"];
            bool click = manuseio_inputs->mouse_input.botoes["left"] > 0;

            if (mouse_state == click)
            {
                switch (type)
                {
                case ui_type::button:
                    function_button();
                    break;

                case ui_type::input_text:
                    function_input_text(input_text);
                    break;

                case ui_type::input_number:
                    function_input_number(input_number);
                    break;

                default:
                    break;
                }
            }
        }
    }

    ~ui_element() {}
};
