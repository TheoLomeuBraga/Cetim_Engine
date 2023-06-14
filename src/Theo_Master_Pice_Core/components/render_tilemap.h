#pragma once

#include <iostream>
#include <functional>
using namespace std;


#include "RecursosT.h"
#include "game_object.h"

class render_tilemap : public componente {
	public:
		render_tilemap(){}
		Material mat;
		bool ligado = true;
		uint8_t camada = 0;
		int apenas_camada = -1;
		shared_ptr<tile_set> tiles;
		shared_ptr<tile_map_info> map_info;
		


	};