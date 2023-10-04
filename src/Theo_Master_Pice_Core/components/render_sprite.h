#pragma once

#include <iostream>
#include <functional>
using namespace std;

#include "RecursosT.h"
#include "game_object.h"

class render_sprite : public componente
{
public:
	render_sprite() {}
	Material mat;
	bool ligado = true;
	uint8_t camada = 0;
	uint8_t tile_selecionado;
	shared_ptr<tile_set> tiles;
};

#include "ecs/ecs.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <any>

namespace ecs_render_sprite
{

	struct render_sprite_data
	{
		bool on = true;
		Material mat;
		uint8_t layer = 0;
		uint8_t tile_selecionado;
		shared_ptr<tile_set> tiles;
	};

	std::map<entity, struct render_sprite_data> render_sprites_map;

	void add_render_sprite(entity id, std::any data)
	{
		render_sprites_map.insert(std::pair<entity, struct render_sprite_data>(id, std::any_cast<struct render_sprite_data>(data)));
	}

	bool have_render_sprite(entity id)
	{
		if (render_sprites_map.find(id) != render_sprites_map.end())
		{
			return true;
		}
		return false;
	}

	void run_render_sprite(entity id) {}

	void run_render_sprites() {}

	void remove_render_sprite(entity id)
	{
		render_sprites_map.erase(id);
	}

	void register_render_sprite_component()
	{
		ecs_systems_registerd.insert(std::pair<std::string, struct ecs_system>("render_sprite", {add_render_sprite, have_render_sprite, run_render_sprite, run_render_sprites, remove_render_sprite}));
	}
};