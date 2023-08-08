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
		bool uniform_space_between_characters = false;
		float espaco_entre_linhas = 3;
		vec2 text_size = vec2(0,0);
		wstring texto;

		void set_text_by_string(string text){
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    		texto = converter.from_bytes(text.c_str());
		}


		render_texto() {}



	};