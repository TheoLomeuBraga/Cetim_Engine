#pragma once

#include <iostream>
#include <functional>
using namespace std;


#include "RecursosT.h"
#include "game_object.h"

class render_texto : public componente {
	public:
		Material mat;
		shared_ptr<fonte> font;
		bool ligado = true;
		uint8_t camada = 0;
		float tamanho_max_linha = 34;
		float tamanho_espaco = 1;
		float espaco_entre_letras = 0.5f;
		float espaco_entre_letras_max = 3.0f, espaco_entre_letras_min = 0.0f;
		float espaco_entre_linhas = 3;
		string texto = "";





		render_texto() {}



	};