#pragma once

#include <string>

#include "implementacao_glfw.h"
#include "OpenGL.h"
#include "implementacao_glfw.h"

#include "shared_library_loader.h"

typedef void(*init_lib_function)(void);

init_lib_function start_window_lib = [](){gerente_janela = new gerenciador_janela_glfw(true);};

init_lib_function start_graphcal_lib = [](){api_grafica = new OpenGL_API();};

init_lib_function start_input_lib = [](){manuseio_inputs = new glfw_input_manager();};

void load_custom_libs_from(std::string lib_location){

}