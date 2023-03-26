#pragma once

#include <iostream>
#include <functional>
using namespace std;


#include "RecursosT.h"
#include "components/light.h"
#include "components/render_mesh.h"
#include "components/render_sprite.h"
#include "components/render_tilemap.h"
#include "components/render_text.h"
#include "components/render_shader.h"
#include "components/camera.h"
#include "components/transform.h"
#include "game_object.h"






vector<shared_ptr<objeto_jogo>> lixeira;
	void esvasiar_lixeira() {
		vector<shared_ptr<objeto_jogo>> vazio;
		lixeira.swap(vazio);
	}

	shared_ptr<objeto_jogo> objeto_cena = novo_objeto_jogo(true);
	

	class cena_objetos {
	public:
		string nome;
		
		vector<shared_ptr<objeto_jogo>> lista_objetos;
		
	
		
		vector<int> cameras_id;
		vector<shared_ptr<objeto_jogo>> cameras;
		

		vector<int> fontes_luzes_id;
		
		

		
		vector<vector<int>> objetos_camadas_render_id;
		vector<vector<shared_ptr<objeto_jogo>>> objetos_camadas_render;
		

		shared_ptr<objeto_jogo> operator [] (int i) { return lista_objetos[i]; }

		shared_ptr<objeto_jogo> operator [] (string s) { return pegar_objeto_nome(s); }

		void remover_objeto(shared_ptr<objeto_jogo> obj) {
			for (shared_ptr<objeto_jogo> f : obj->filhos) {
				remover_objeto(f);
			}

			if (obj->pai != NULL) {
				obj->pai->childrens_to_remove.push_back(obj.get());
			}
			
			obj->lixo = true;
			lixeira.push_back(obj);
			obj->finalisar();
			obj->marcar_em_cena_como(false);
			obj->desconectar_componentes();
		}

		void remover_objeto(objeto_jogo* obj) {

		}

		void remover_objetos(vector<shared_ptr<objeto_jogo>> objs) {
			for (shared_ptr<objeto_jogo> obj : objs) { remover_objeto(obj); }
		}

		void adicionar_objeto_lista(shared_ptr<objeto_jogo> obj) {
			if (obj != NULL) {
				obj->remove_childrens_to_remove();
				if (obj->lixo) {
					
					
					remover_objeto(obj);
					

				}
				else {

					
					obj->em_cena = true;



					obj->ID = lista_objetos.size();
					lista_objetos.push_back(obj);

					//camera
					void* componente[6];
					componente[0] = obj->pegar_componente<camera>().get();
					if (componente[0] != NULL) {
						cameras_id.push_back(obj->ID);
						cameras.push_back(obj);
					}









					//objetos_camadas_render_id
					shared_ptr<render_shader> rs = obj->pegar_componente<render_shader>();
					if ((rs != NULL && rs->ligado)) {
						if (objetos_camadas_render_id.size() < (rs->camada + 1)) { objetos_camadas_render_id.resize(rs->camada + 1); }
						objetos_camadas_render_id[rs->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (rs->camada + 1)) { objetos_camadas_render.resize(rs->camada + 1); }
						objetos_camadas_render[rs->camada].push_back(obj);
					}
					shared_ptr < render_texto> RT = obj->pegar_componente<render_texto>();
					if ((RT != NULL && RT->ligado)) {
						if (objetos_camadas_render_id.size() < (RT->camada + 1)) { objetos_camadas_render_id.resize(RT->camada + 1); }
						objetos_camadas_render_id[RT->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RT->camada + 1)) { objetos_camadas_render.resize(RT->camada + 1); }
						objetos_camadas_render[RT->camada].push_back(obj);
					}
					shared_ptr < render_tilemap> RTM = obj->pegar_componente<render_tilemap>();
					if ((RTM != NULL && RTM->ligado)) {
						if (objetos_camadas_render_id.size() < (RTM->camada + 1)) { objetos_camadas_render_id.resize(RTM->camada + 1); }
						objetos_camadas_render_id[RTM->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RTM->camada + 1)) { objetos_camadas_render.resize(RTM->camada + 1); }
						objetos_camadas_render[RTM->camada].push_back(obj);
					}
					shared_ptr < render_sprite> RS = obj->pegar_componente<render_sprite>();
					if ((RS != NULL && RS->ligado)) {
						if (objetos_camadas_render_id.size() < (RS->camada + 1)) { objetos_camadas_render_id.resize(RS->camada + 1); }
						objetos_camadas_render_id[RS->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RS->camada + 1)) { objetos_camadas_render.resize(RS->camada + 1); }
						objetos_camadas_render[RS->camada].push_back(obj);
					}

					shared_ptr < render_malha> RM = obj->pegar_componente<render_malha>();
					if ((RM != NULL && RM->ligado)) {
						if (objetos_camadas_render_id.size() < (RM->camada + 1)) { objetos_camadas_render_id.resize(RM->camada + 1); }
							objetos_camadas_render_id[RM->camada].push_back(obj->ID);

							if (objetos_camadas_render.size() < (RM->camada + 1)) { objetos_camadas_render.resize(RM->camada + 1); }
							objetos_camadas_render[RM->camada].push_back(obj);
					}
					shared_ptr <fonte_luz> FL = obj->pegar_componente<fonte_luz>();
					if ((FL != NULL && FL->ligado)) {
						fontes_luzes_id.push_back(obj->ID);
					}



					for (shared_ptr<objeto_jogo> ob : obj->filhos) {
						adicionar_objeto_lista(ob);
					}


				}
			}
		}

		shared_ptr<objeto_jogo> pegar_objeto_nome(string n) {
			shared_ptr<objeto_jogo> ret = NULL;
			for (shared_ptr<objeto_jogo> obj : lista_objetos) {
				if (!obj->nome.compare(n)) {
					return obj;
				}
			}
			return ret;
		}

		
		void atualisar() {

			

				esvasiar_lixeira();



				lista_objetos.clear();
				cameras_id.clear();
				objetos_camadas_render_id.clear();
				fontes_luzes_id.clear();
				cameras.clear();
				objetos_camadas_render.clear();

				adicionar_objeto_lista(objeto_cena);




				






		}

		void atualisar_transform_objeto(shared_ptr<objeto_jogo> obj) {
			shared_ptr<transform_> TF = obj->pegar_componente<transform_>();
			if (TF != NULL) {
				TF->atualizar_tf();
			}

			shared_ptr < camera> cam = obj->pegar_componente<camera>();
			if (cam != NULL) {
				cam->paiTF = TF.get();
				cam->atualizar_tf();
			}




			for (shared_ptr<objeto_jogo> ob : obj->filhos) {
				shared_ptr<transform_> TF_filho = ob->pegar_componente<transform_>();
				if (TF_filho != NULL) {
					TF_filho->paiTF = TF.get();
				}

				atualisar_transform_objeto(ob);
			}
		}

		void atualisar_transforms() {
			atualisar_transform_objeto(objeto_cena);

		}


		

		//adicionar objeto
		void adicionar_objeto(shared_ptr<objeto_jogo> pai, shared_ptr<objeto_jogo> obj){

			obj->marcar_em_cena_como(true);
			pai->filhos.push_back(obj);
			obj->pai = pai.get();
			obj->adicionar_cena();


			
		}

		void adicionar_objetos(shared_ptr<objeto_jogo> pai,vector<shared_ptr<objeto_jogo>> objs) {
			for (shared_ptr<objeto_jogo> obj : objs) {
				adicionar_objeto(pai, obj);
			}
		}

		void adicionar_objeto(objeto_jogo* pai, shared_ptr<objeto_jogo> obj) {

			obj->marcar_em_cena_como(true);
			pai->filhos.push_back(obj);
			obj->pai = pai;
			obj->adicionar_cena();



		}

		void adicionar_objetos(objeto_jogo* pai, vector<shared_ptr<objeto_jogo>> objs) {
			for (shared_ptr<objeto_jogo> obj : objs) {
				adicionar_objeto(pai, obj);
			}
		}
		
		void adicionar_objeto(shared_ptr<objeto_jogo> obj) { adicionar_objeto(objeto_cena, obj); }

		void adicionar_objetos(vector<shared_ptr<objeto_jogo>> objs) { adicionar_objetos(objeto_cena, objs); }
		
		

		

		

		//objetos[a]->scriptsCpp
		void iniciar_Logica_Scripst() {
			for (shared_ptr<objeto_jogo> obj : lista_objetos) {
				obj->iniciar();
			}
		}

		void atualisar_Logica_Scripst() {
			for (shared_ptr<objeto_jogo> obj : lista_objetos) {
				if (obj != NULL) {
					obj->atualisar();
				}
			}
		}

		





		cena_objetos(string n) {
			nome = n;
			adicionar_objeto(novo_objeto_jogo());
		}



	};



	






	cena_objetos* cena_objetos_selecionados;
	//cena_objetos cena_objetos_selecionados;
