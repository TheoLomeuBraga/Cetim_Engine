#pragma once

#include <iostream>

#include <algorithm>
#include <functional>
#include <thread>

// using namespace std;

// #include "imgui.cpp"
// #include "imgui_draw.cpp"
// #include "imgui_tables.cpp"
// #include "imgui_widgets.cpp"
// #include "imgui_impl_opengl3.cpp"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_glfw.cpp"
// #include "imgui_impl_glfw.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
#include "components/box_2d.h"
#include "components/implementacao_lua.h"
#include "implementacao_windows.cpp"

// APIs
#include "OpenGL.h"
#include "implementacao_glfw.h"
#include "components/sfml_audio.h"
#include "game_object.h"

#include "components/camera.h"
#include "components/transform.h"
// #include "Box2D_classe.h"
// #include  "box2D_antigo.h"

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
	ImGui_ImplOpenGL3_Init((const char*)"#version 460 core");
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

	// camera_teste = novo_objeto_jogo();
	funcoes_OS = new foncoes_Windows();
	api_grafica = new OpenGL_API();
	api_grafica->pos_processamento_info.shad = "recursos/Shaders/post_processing";

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

	api_grafica->pos_processamento_info.shad = "resources/Shaders/post_processing";
	api_grafica->pos_processamento_info.cor = vec4(1, 1, 1, 1);
	api_grafica->pos_processamento_info.gama = 1;

	// iniciar libs

	escrever("GPU: " + api_grafica->modelo_gpu);

	core = novo_objeto_jogo();
	core->adicionar_componente<componente_lua>(componente_lua());
	core->pegar_componente<componente_lua>()->adicionar_script("core");
	cena_objetos_selecionados = &cena;
	cena_objetos_selecionados->adicionar_objeto(core);

	
	
	/*
	Material mat;
	mat.shad = "resources/Shaders/mesh";
	mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");

	marching_cubes::Mesh mcm = marching_cubes_test2()[0];
	malha m;

	print({"mcm.indices.size()",mcm.indices.size()});

	for(unsigned int i : mcm.indices){
		m.indice.push_back(i);
	}

	for(marching_cubes::vertex v : mcm.vertces){
		vertice vert;

		vert.posicao[0] = v.position.x;
		vert.posicao[1] = v.position.y;
		vert.posicao[2] = v.position.z;

		vert.uv[0] = v.texcoord.x;
		vert.uv[1] = v.texcoord.y;

		vert.normal[0] = v.normal.x;
		vert.normal[1] = v.normal.y;
		vert.normal[2] = v.normal.z;

		m.vertices.push_back(vert);
	}

	test_marching_cube = novo_objeto_jogo();
	test_marching_cube->adicionar_componente<transform_>();

	test_marching_cube->adicionar_componente<render_malha>();
	shared_ptr<render_malha> rmp = test_marching_cube->pegar_componente<render_malha>();

	rmp->malhas = {};
	rmp->mats = {mat};
	cena_objetos_selecionados->adicionar_objeto(test_marching_cube);
	*/
}

shared_ptr<objeto_jogo> obj;
void teste3()
{
	// cout << "instancias: " << objeto_jogo::instancias << endl;
}
