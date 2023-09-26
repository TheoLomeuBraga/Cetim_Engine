#pragma once

#include <iostream>
using namespace std;
#include "RecursosT.h"
#include "game_object.h"
#include "implementacao_glfw.h"
#include "transform.h"

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

class ui_mouse_cheker : public componente
{
public:
    shared_ptr<transform_> tf = NULL;
    unsigned char type = 0;
    unsigned char mouse_state = 0;
    string function_lua_id = "";
    void(function_button)(void);

    string input_text = "";
    float input_number = 0;

    void(function_input_text)(string);
    void(function_input_number)(float);

    ui_mouse_cheker() {}

    void atualisar()
    {

        tf = esse_objeto->pegar_componente<transform_>();

        if (tf != NULL)
        {
            if (type > 0)
            {

                

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
    }

    ~ui_mouse_cheker() {}
};
