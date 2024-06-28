#pragma once

#include <iostream>
#include <functional>
using String = std::string;

#include "RecursosT.h"
#include "game_object.h"

class render_malha : public componente
{
public:
	render_malha() {
		should_update = false;
	}
	bool ligado = true;
	// char lado_render = lado_render_malha::front;
	bool usar_oclusao = true;
	float porcentagem_pode_ocupar_tela = 1;
	uint8_t camada = 0;
	vector<shared_ptr<malha>> malhas;
	vector<Material> mats;
	vector<objeto_jogo*> bones;
	unsigned int render_count = 1;
};

#include "ecs/ecs.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <any>

namespace ecs_render_mesh
{

	struct render_mesh_data
	{
		bool on = true;
		bool use_oclusion = true;
		float screen_usage_percentage = 1;
		uint8_t layer = 0;
		vector<shared_ptr<malha>> meshes;
		vector<Material> materials;
	};

	std::map<entity, struct render_mesh_data> render_meshs_map;

	void add_render_mesh(entity id, std::any data)
	{
		render_meshs_map.insert(std::pair<entity, struct render_mesh_data>(id, std::any_cast<struct render_mesh_data>(data)));
	}

	bool have_render_mesh(entity id)
	{
		if (render_meshs_map.find(id) != render_meshs_map.end())
		{
			return true;
		}
		return false;
	}

	void run_render_mesh(entity id) {}

	void run_render_meshs() {}

	void remove_render_mesh(entity id)
	{
		render_meshs_map.erase(id);
	}

	void register_render_mesh_component()
	{
		ecs_systems_registerd.insert(std::pair<std::string, struct ecs_system>("render_mesh", {add_render_mesh, have_render_mesh, run_render_mesh, run_render_meshs, remove_render_mesh}));
	}
};
