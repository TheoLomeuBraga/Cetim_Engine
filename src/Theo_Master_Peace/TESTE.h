#pragma once

#include <iostream>

#include<algorithm>
#include <functional>
#include <thread>



//using namespace std;

//#include "imgui.cpp"
//#include "imgui_draw.cpp"
//#include "imgui_tables.cpp"
//#include "imgui_widgets.cpp"
//#include "imgui_impl_opengl3.cpp"
//#include "imgui_impl_opengl3.h"
//#include "imgui_impl_glfw.cpp"
//#include "imgui_impl_glfw.h"









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

//APIs
#include "OpenGL.h"
#include "implementacao_glfw.h"
#include "components/sfml_audio.h"
#include "game_object.h"

#include "components/camera.h"
#include "components/transform.h"
//#include "Box2D_classe.h"
//#include  "box2D_antigo.h"


using namespace loop_principal;
//teste3
cena_objetos cena("Teste3");


Material* mate;
imagem* imagemtexte3;
imagem* imagem2texte3;
info_camada ca;

objeto_jogo  *teste_memoria_obj_pai = new objeto_jogo();
objeto_jogo* fundo = new objeto_jogo();



/*
vector<objeto_jogo*> tilemap_objetos(shared_ptr<tile_set> ts, shared_ptr < tile_map_info> tm) {
	vector<objeto_jogo*> ret;

	

	

	for (int a = 0; a < tm->info.size(); a++) {
		for (int b = 0; b < tm->info[a].size(); b++) {


			int tile_id = 0;
			if (tm->info[a][b] - 1 >= 0) {
				tile_id = tm->info[a][b] - 1;
			}
			
			if (ts->tiles[tile_id].tipo_colisao != 0 ) {
				objeto_jogo* obj = new objeto_jogo();

				

				vec3 pos = (ivec3(b % tm->res.x, (float)(int)b / tm->res.x, a) * ivec3(2, -2, 0));
				obj->adicionar_componente<transform>(transform(false, pos, vec3(0, 0, 0), vec3(1, 1, 1)));
				
				if (ts->tiles[tile_id].tipo_colisao != 0) {
					box_2D b2d;
					b2d.atrito = 0;
					b2d.forma = formato_colisao::tile_suave;
					obj->adicionar_componente<box_2D>(b2d);
					
				}

				if (ts->tiles[tile_id].interativel != 0) {
					render_sprite rs;
					rs.tiles = ts;
					
					rs.mat.shad = "resources\\Shaders\\sprite";
					rs.tile_selecionado = tile_id;
					obj->adicionar_componente<render_sprite>(rs);
				}

				ret.push_back(obj);

			}
		}
	}
	return ret;
}








*/




unsigned int  local_fb_editor;
void inciar_imgui() {
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
void antes_imgui() {
	/*
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	*/
}
void depois_imgui() {
	/*
	ImGui::Begin("info");
	ImGui::Text(string(string("FPS: ") + to_string(Tempo::FPS)).c_str());
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	*/
	
}


shared_ptr<objeto_jogo> core;

//shared_ptr<objeto_jogo> camera_teste;

void iniciarTeste3() {
	
	//camera_teste = novo_objeto_jogo();
	funcoes_OS = new foncoes_Windows();
	api_grafica = new OpenGL_API();
	api_grafica->pos_processamento_info.shad = "recursos/Shaders/post_processing";


	MatrizMundi = mat4(1.0f);


	
	instrucoes_render ir1, ir2,ir3, ir4;

	ir2.usar_profundidade = false;
	ir2.limpar_buffer_cores = false;
	ir2.iniciar_render = false;
	//ir2.desenhar_objetos = false;
	
	ir3.limpar_buffer_cores = false;
	ir3.iniciar_render = false;
	//ir3.desenhar_objetos = false;
	
	ir4.usar_profundidade = false;
	ir4.limpar_buffer_cores = false;
	ir4.iniciar_render = false;
	

	api_grafica->info_render.resize(4);
	api_grafica->info_render[0] = ir1;
	api_grafica->info_render[1] = ir2;
	api_grafica->info_render[2] = ir3;
	api_grafica->info_render[3] = ir4;
	

	ca.camada = 0;
	ca.camada_colide = {0,};
	

	
	


	//pos processamento


	api_grafica->pos_processamento_info.shad = "resources/Shaders/post_processing";
	api_grafica->pos_processamento_info.cor = vec4(1, 1, 1, 1);
	api_grafica->pos_processamento_info.gama = 1;
	
	//iniciar libs
	

	escrever("GPU selecionada: " + api_grafica->modelo_gpu);





	core = novo_objeto_jogo();
	core->adicionar_componente<componente_lua>(componente_lua());
	core->pegar_componente<componente_lua>()->adicionar_script("core");
	
	cena_objetos_selecionados = &cena;
	
	
	
	
	
	
	cena_objetos_selecionados->adicionar_objeto(core);
	

	

}











shared_ptr<objeto_jogo> obj;
void teste3() {
	//cout << "instancias: " << objeto_jogo::instancias << endl;
	







}






