#pragma once

#include <string>
#include <vector>

#include "implementacao_glfw.h"
#include "OpenGL.h"
#include "implementacao_glfw.h"

#include "shared_library_loader.h"

std::vector<Shared_Library_Loader> slls;

typedef void(*init_lib_function)(void);

//init_lib_function start_window_lib = [](){gerente_janela = new gerenciador_janela_glfw(true);};
init_lib_function start_window_lib = [](){gerente_janela = new gerenciador_janela_sdl();};

//init_lib_function start_input_lib = [](){manuseio_inputs = new glfw_input_manager();};
init_lib_function start_input_lib = [](){manuseio_inputs = new  input_manager_sdl();};

init_lib_function start_graphcal_lib = [](){api_grafica = new OpenGL_API();};

void load_custom_libs_from(std::string lib_location){

}