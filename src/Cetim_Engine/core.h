#pragma once

#include <iostream>

#include <algorithm>
#include <functional>
#include <thread>

// using String = std::string;

// #include "imgui.cpp"
// #include "imgui_draw.cpp"
// #include "imgui_tables.cpp"
// #include "imgui_widgets.cpp"
// #include "imgui_impl_opengl3.cpp"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_glfw.cpp"
// #include "imgui_impl_glfw.h"


#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
using namespace glm;

#include "RecursosT.h"
#include "ManusearArquivos.h"
#include "Config.h"
#include "input.h"
#include "Tempo.h"
#include "scene.h"
#include "LoopPrincipal.h"
#include "box_2d.h"
#include "implementacao_lua.h"

// APIs
#include "OpenGL.h"
#include "implementacao_glfw.h"
#include "audio_source.h"
#include "game_object.h"

#include "camera.h"
#include "transform.h"
// #include "Box2D_classe.h"
// #include  "box2D_antigo.h"

#include "ui_element.h" 

using namespace loop_principal;
// teste3
cena_objetos cena("Teste3");

Material *mate;
imagem *imagemtexte3;
imagem *imagem2texte3;
info_camada ca;

objeto_jogo *teste_memoria_obj_pai = new objeto_jogo();
objeto_jogo *fundo = new objeto_jogo();

#include "marching_cubes/marching_cubes_test.h"

#include "init_lib_functions.h"

unsigned int local_fb_editor;
void inciar_imgui()
{
	/*
	OpenGL_API* a = (OpenGL_API*)api_grafica;
	local_fb_editor = a->frame_buffer_editor_texture;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(janela, true);
	ImGui_ImplOpenGL3_Init((const char*)"#version 330 core");
	ImGui::StyleColorsDark();
	*/
}
void antes_imgui()
{
	/*
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	*/
}
void depois_imgui()
{
	/*
	ImGui::Begin("info");
	ImGui::Text(string(string("FPS: ") + to_string(Tempo::FPS)).c_str());
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	*/
}

shared_ptr<objeto_jogo> core, test_marching_cube;

// shared_ptr<objeto_jogo> camera_teste;

void iniciarTeste3()
{
	start_graphcal_lib();
	start_input_lib();
	
	api_grafica->pos_processamento_info.shader = "recursos/Shaders/post_processing";

	MatrizMundi = mat4(1.0f);

	instrucoes_render ir1, ir2, ir3, ir4;

	ir2.usar_profundidade = false;
	ir2.limpar_buffer_cores = false;
	ir2.iniciar_render = false;
	// ir2.desenhar_objetos = false;

	ir3.limpar_buffer_cores = false;
	ir3.iniciar_render = false;
	// ir3.desenhar_objetos = false;

	ir4.usar_profundidade = false;
	ir4.limpar_buffer_cores = false;
	ir4.iniciar_render = false;

	api_grafica->info_render.resize(4);
	api_grafica->info_render[0] = ir1;
	api_grafica->info_render[1] = ir2;
	api_grafica->info_render[2] = ir3;
	api_grafica->info_render[3] = ir4;

	ca.camada = 0;
	ca.camada_colide = {
		0,
	};

	// pos processamento

	api_grafica->pos_processamento_info.shader = "post_processing";
	api_grafica->pos_processamento_info.cor = vec4(1, 1, 1, 1);
	api_grafica->pos_processamento_info.gama = 1;

	// iniciar libs

	//print("GPU:",api_grafica->modelo_gpu);

	core = novo_objeto_jogo();
	core->adicionar_componente<componente_lua>(componente_lua());
	core->pegar_componente<componente_lua>()->adicionar_script("core");
	cena_objetos_selecionados = &cena;
	cena_objetos_selecionados->adicionar_objeto(core);
	
	//test_ui();
}

shared_ptr<objeto_jogo> obj;
void teste3()
{
}
