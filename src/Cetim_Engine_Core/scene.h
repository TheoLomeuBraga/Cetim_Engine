#pragma once

#include <iostream>
#include <functional>
#include <thread>
using String = std::string;

#include "RecursosT.h"
#include "light.h"
#include "render_mesh.h"
#include "render_sprite.h"
#include "render_tilemap.h"
#include "render_text.h"
#include "render_shader.h"
#include "camera.h"
#include "transform.h"
#include "poly_mesh.h"
#include "game_object.h"
#include "benchmark.h"

vector<shared_ptr<objeto_jogo>> lixeira;
void esvasiar_lixeira()
{
	vector<shared_ptr<objeto_jogo>> vazio;
	lixeira.swap(vazio);
}

shared_ptr<objeto_jogo> objeto_cena = novo_objeto_jogo(true);

class cena_objetos
{
public:
	string nome;

	vector<shared_ptr<objeto_jogo>> lista_objetos;

	vector<int> cameras_id;
	vector<shared_ptr<objeto_jogo>> cameras;

	vector<int> fontes_luzes_id;

	vector<vector<int>> objetos_camadas_render_id;
	vector<vector<shared_ptr<objeto_jogo>>> objetos_camadas_render;

	shared_ptr<objeto_jogo> operator[](int i) { return lista_objetos[i]; }

	shared_ptr<objeto_jogo> operator[](string s) { return pegar_objeto_nome(s); }

	void remover_objeto(shared_ptr<objeto_jogo> obj)
	{
		for (shared_ptr<objeto_jogo> f : obj->filhos)
		{
			remover_objeto(f);
		}

		if (obj->pai != NULL)
		{
			obj->pai->childrens_to_remove.push_back(obj.get());
		}

		obj->lixo = true;
		lixeira.push_back(obj);
		obj->finalisar();
		obj->marcar_em_cena_como(false);
		obj->desconectar_componentes();
	}

	void remover_objeto(objeto_jogo *obj)
	{
		remover_objeto(obj->get_this_object());
	}

	void remover_objetos(vector<shared_ptr<objeto_jogo>> objs)
	{
		for (shared_ptr<objeto_jogo> obj : objs)
		{
			remover_objeto(obj);
		}
	}

	void adicionar_objeto_lista(shared_ptr<objeto_jogo> obj)
	{
		
		if (obj != NULL)
		{
			obj->remove_childrens_to_remove();
			if (obj->lixo)
			{

				remover_objeto(obj);
			}
			else
			{

				obj->em_cena = true;

				obj->ID = lista_objetos.size();
				lista_objetos.push_back(obj);

				///*
				// render shader
				shared_ptr<render_sprite> RS = obj->pegar_componente<render_sprite>();
				shared_ptr<poly_mesh> PMESH = obj->pegar_componente<poly_mesh>();
				shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
				if ((RS != NULL && RS->ligado))
				{
					if (objetos_camadas_render_id.size() < (RS->camada + 1))
					{
						objetos_camadas_render_id.resize(RS->camada + 1);
					}
					objetos_camadas_render_id[RS->camada].push_back(obj->ID);

					if (objetos_camadas_render.size() < (RS->camada + 1))
					{
						objetos_camadas_render.resize(RS->camada + 1);
					}
					objetos_camadas_render[RS->camada].push_back(obj);
				}


				else if ((PMESH != NULL && PMESH->ligado))
				{
					if (objetos_camadas_render_id.size() < (PMESH->camada + 1))
					{
						objetos_camadas_render_id.resize(PMESH->camada + 1);
					}
					objetos_camadas_render_id[PMESH->camada].push_back(obj->ID);

					if (objetos_camadas_render.size() < (PMESH->camada + 1))
					{
						objetos_camadas_render.resize(PMESH->camada + 1);
					}
					objetos_camadas_render[PMESH->camada].push_back(obj);
				}

				
				else if (tf != NULL && obj->get_components_count() > 1)
				{

					// camera
					shared_ptr<camera> cam = obj->pegar_componente<camera>();
					if (cam != NULL)
					{
						cameras_id.push_back(obj->ID);
						cameras.push_back(obj);
					}

					// objetos_camadas_render_id
					shared_ptr<render_shader> rs = obj->pegar_componente<render_shader>();
					shared_ptr<render_texto> RT = obj->pegar_componente<render_texto>();
					shared_ptr<render_tilemap> RTM = obj->pegar_componente<render_tilemap>();
					shared_ptr<render_malha> RM = obj->pegar_componente<render_malha>();
					shared_ptr<fonte_luz> FL = obj->pegar_componente<fonte_luz>();
					if ((rs != NULL && rs->ligado))
					{
						if (objetos_camadas_render_id.size() < (rs->camada + 1))
						{
							objetos_camadas_render_id.resize(rs->camada + 1);
						}
						objetos_camadas_render_id[rs->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (rs->camada + 1))
						{
							objetos_camadas_render.resize(rs->camada + 1);
						}
						objetos_camadas_render[rs->camada].push_back(obj);
					}
					
					else if ((RT != NULL && RT->ligado))
					{
						if (objetos_camadas_render_id.size() < (RT->camada + 1))
						{
							objetos_camadas_render_id.resize(RT->camada + 1);
						}
						objetos_camadas_render_id[RT->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RT->camada + 1))
						{
							objetos_camadas_render.resize(RT->camada + 1);
						}
						objetos_camadas_render[RT->camada].push_back(obj);
					}
					
					else if ((RTM != NULL && RTM->ligado))
					{
						if (objetos_camadas_render_id.size() < (RTM->camada + 1))
						{
							objetos_camadas_render_id.resize(RTM->camada + 1);
						}
						objetos_camadas_render_id[RTM->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RTM->camada + 1))
						{
							objetos_camadas_render.resize(RTM->camada + 1);
						}
						objetos_camadas_render[RTM->camada].push_back(obj);
					}

					
					else if ((RM != NULL && RM->ligado))
					{
						if (objetos_camadas_render_id.size() < (RM->camada + 1))
						{
							objetos_camadas_render_id.resize(RM->camada + 1);
						}
						objetos_camadas_render_id[RM->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RM->camada + 1))
						{
							objetos_camadas_render.resize(RM->camada + 1);
						}
						objetos_camadas_render[RM->camada].push_back(obj);
					}
					//*/
					
					else if ((FL != NULL && FL->ligado))
					{
						fontes_luzes_id.push_back(obj->ID);
					}
				}

				for (shared_ptr<objeto_jogo> ob : obj->filhos)
				{
					adicionar_objeto_lista(ob);
				}
			}
		}
	}

	shared_ptr<objeto_jogo> pegar_objeto_nome(string n)
	{
		shared_ptr<objeto_jogo> ret = NULL;
		for (shared_ptr<objeto_jogo> obj : lista_objetos)
		{
			if (!obj->nome.compare(n))
			{
				return obj;
			}
		}
		return ret;
	}

	void ordenar_luzes()
	{
		std::vector<shared_ptr<objeto_jogo>> objs;
		vector<int> novo_fontes_luzes_id;
		if (cameras.size() > 0)
		{
			vec3 pos = cameras[0]->pegar_componente<transform_>()->pos;
			for (int i = 0; i < fontes_luzes_id.size(); i++)
			{
				objs.push_back(lista_objetos[fontes_luzes_id[i]]);
			}

			objs = tf_ordenate_by_distance(pos, objs);

			for (int i = 0; i < objs.size(); i++)
			{
				novo_fontes_luzes_id.push_back(objs[i]->ID);
			}

			fontes_luzes_id.swap(novo_fontes_luzes_id);
		}
	}

	void clean_lists()
	{
		lista_objetos.clear();
		cameras_id.clear();
		objetos_camadas_render_id.clear();
		fontes_luzes_id.clear();
		cameras.clear();
		objetos_camadas_render.clear();
	}

	void atualisar()
	{
		/*
		thread t1(esvasiar_lixeira);
		thread t2(&cena_objetos::clean_lists,this);
		
		t1.join();
		t2.join();
		*/

		esvasiar_lixeira();
		clean_lists();

		adicionar_objeto_lista(objeto_cena);
		ordenar_luzes();
	}

	void atualisar_transform_objeto(shared_ptr<objeto_jogo> obj)
	{
		shared_ptr<transform_> TF = obj->pegar_componente<transform_>();
		if (TF != NULL)
		{
			TF->atualizar_tf();
		}

		shared_ptr<camera> cam = obj->pegar_componente<camera>();
		if (cam != NULL)
		{
			cam->paiTF = TF;
			cam->atualizar_tf();
		}

		for (shared_ptr<objeto_jogo> ob : obj->filhos)
		{
			shared_ptr<transform_> TF_filho = ob->pegar_componente<transform_>();
			if (TF_filho != NULL)
			{
				TF_filho->paiTF = TF.get();
			}

			atualisar_transform_objeto(ob);
		}
	}

	void atualisar_transforms()
	{
		atualisar_transform_objeto(objeto_cena);
	}

	// adicionar objeto
	void adicionar_objeto(shared_ptr<objeto_jogo> pai, shared_ptr<objeto_jogo> obj)
	{

		obj->marcar_em_cena_como(true);
		pai->filhos.push_back(obj);
		obj->pai = pai.get();
		obj->adicionar_cena();
	}

	void adicionar_objetos(shared_ptr<objeto_jogo> pai, vector<shared_ptr<objeto_jogo>> objs)
	{
		for (shared_ptr<objeto_jogo> obj : objs)
		{
			adicionar_objeto(pai, obj);
		}
	}

	void adicionar_objeto(objeto_jogo *pai, shared_ptr<objeto_jogo> obj)
	{

		obj->marcar_em_cena_como(true);
		pai->filhos.push_back(obj);
		obj->pai = pai;
		obj->adicionar_cena();
	}

	void adicionar_objetos(objeto_jogo *pai, vector<shared_ptr<objeto_jogo>> objs)
	{
		for (shared_ptr<objeto_jogo> obj : objs)
		{
			adicionar_objeto(pai, obj);
		}
	}

	void adicionar_objeto(shared_ptr<objeto_jogo> obj) { adicionar_objeto(objeto_cena, obj); }

	void adicionar_objetos(vector<shared_ptr<objeto_jogo>> objs) { adicionar_objetos(objeto_cena, objs); }

	// objetos[a]->scriptsCpp
	void iniciar_Logica_Scripst()
	{
		for (shared_ptr<objeto_jogo> obj : lista_objetos)
		{
			obj->iniciar();
		}
	}

	void atualisar_Logica_Scripst()
	{
		for (shared_ptr<objeto_jogo> obj : lista_objetos)
		{
			obj->atualisar();
		}
	}

	cena_objetos(string n)
	{
		nome = n;
		adicionar_objeto(novo_objeto_jogo());
	}
};

cena_objetos *cena_objetos_selecionados;
// cena_objetos cena_objetos_selecionados;
