#pragma once

#include <iostream>
#include <functional>
using namespace std;


#include "RecursosT.h"
#include "game_object.h"

class render_sprite : public componente {
	public:
		render_sprite() {}
		Material mat;
		bool ligado = true;
		uint8_t camada = 0;
		uint16_t tile_selecionado;
		shared_ptr<tile_set> tiles;



	};