#pragma once


#include <iostream>
#include <codecvt>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <string>
#include <locale>


#include <functional>
using namespace std;

#include "RecursosT.h"
#include "game_object.h"


enum render_text_location
{
	NONE = 0,
	CENTER = 1,
	TOP = 2,
	DOWN = 3,
	LEFT = 4,
	RIGHT = 5

};

std::wstring convert_to_wstring(const std::string& text) {
    // Cria um objeto de conversão de caracteres de UTF-8 para UTF-16
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // Converte a string UTF-8 para wstring UTF-16
    return converter.from_bytes(text);
}

class render_texto : public componente
{
public:
	Material mat;
	shared_ptr<fonte> font;
	bool ligado = true;
	uint8_t camada = 0;
	float tamanho_max_linha = 1000;
	float tamanho_espaco = 1;
	bool uniform_space_between_characters = false;
	float espaco_entre_linhas = 3;
	wstring texto;
	char text_location_x = render_text_location::CENTER;
	char text_location_y = render_text_location::CENTER;

	vec2 get_text_size()
	{
		vec2 ret = vec2(0, 0);

		vec2 pos_char(0, 0), pos_adi_char(0, 0), sca_char(1, 1);
		float altura_linha = 0;
		float tamanho_linha = 0;

		for (int i = 0; i < texto.size(); i++)
		{
			wchar_t letra = texto.at(i);
			if (letra == '\n')
			{
				altura_linha -= +espaco_entre_linhas;
				ret.x = std::max(ret.x,pos_char.x);
				ret.y = std::min(ret.y,altura_linha);
				pos_char.x = 0;
			}
			else if (letra == ' ')
			{
				pos_char.x += tamanho_espaco;
				if (pos_char.x > tamanho_max_linha)
				{
					altura_linha -= +espaco_entre_linhas;
					ret.x = std::max(ret.x,pos_char.x);
					ret.y = std::min(ret.y,altura_linha);
					pos_char.x = 0;
				}
			}
			else
			{

				caractere_info char_ = font->chars[letra];

				sca_char = vec2(char_.width, char_.height);

				vec2 bearing = vec2(char_.left, char_.top);

				if (uniform_space_between_characters)
				{
					pos_adi_char = vec2(1, 0);
				}
				else
				{
					pos_adi_char = vec2((float)char_.adivancement / font->quality, 0);
				}

				pos_char.x += pos_adi_char.x;

				pos_char.y += pos_adi_char.y;

				//ret.x = std::max(ret.x, (pos_char.x + (sca_char.x / font->quality)));
				//ret.y = std::max(ret.y, espaco_entre_linhas);

				if (pos_char.x > tamanho_max_linha)
				{
					altura_linha -= espaco_entre_linhas;
					ret.x = std::max(ret.x,pos_char.x);
					ret.y = std::min(ret.y,altura_linha);
					pos_char.x = 0;
				}
			}
		}
		ret.x = std::max(ret.x,pos_char.x);
		ret.y = std::min(ret.y,altura_linha);
		return ret;
	}

	vector<vec2> get_lines_size()
	{
		vector<vec2> ret = { vec2(0,0) };

		vec2 pos_char(0, 0), pos_adi_char(0, 0), sca_char(1, 1);
		float altura_linha = 0;
		float tamanho_linha = 0;
		int no_linha = 0;

		for (int i = 0; i < texto.size(); i++)
		{
			wchar_t letra = texto.at(i);
			if (letra == '\n')
			{
				altura_linha -= +espaco_entre_linhas;
				ret[no_linha] = vec2(pos_char.x,espaco_entre_linhas);
				ret.push_back(vec2(0,0));
				no_linha++;
				pos_char.x = 0;
			}
			else if (letra == ' ')
			{
				pos_char.x += tamanho_espaco;
				if (pos_char.x > tamanho_max_linha)
				{
					altura_linha -= +espaco_entre_linhas;
					ret[no_linha] = vec2(pos_char.x,espaco_entre_linhas);
					ret.push_back(vec2(0,0));
					no_linha++;
					pos_char.x = 0;
				}
			}
			else
			{

				caractere_info char_ = font->chars[letra];

				sca_char = vec2(char_.width, char_.height);

				vec2 bearing = vec2(char_.left, char_.top);

				if (uniform_space_between_characters)
				{
					pos_adi_char = vec2(1, 0);
				}
				else
				{
					pos_adi_char = vec2((float)char_.adivancement / font->quality, 0);
				}

				pos_char.x += pos_adi_char.x;

				pos_char.y += pos_adi_char.y;

				if (pos_char.x > tamanho_max_linha)
				{
					altura_linha -= +espaco_entre_linhas;
					ret[no_linha] = vec2(pos_char.x,espaco_entre_linhas);
					ret.push_back(vec2(0,0));
					no_linha++;
					pos_char.x = 0;
				}
			}
		}
		ret[no_linha] = vec2(pos_char.x,espaco_entre_linhas);
		ret.push_back(vec2(0,0));
		return ret;
	}


	void set_text_by_string(string text)
	{
		texto = convert_to_wstring(text);
	}

	render_texto() {}
};