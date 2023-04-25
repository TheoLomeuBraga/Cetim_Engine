#pragma once

#include <iostream>
#include <functional>
using namespace std;

#include "RecursosT.h"
#include "game_object.h"

class render_malha : public componente
{
public:
	render_malha() {}
	bool ligado = true;
	char lado_render = lado_render_malha::front;
	bool usar_oclusao = true;
	float porcentagem_pode_ocupar_tela = 1;
	uint8_t camada = 0;
	map<string, float> shape_keys_state;
	vector<shared_ptr<malha>> malhas;
	vector<Material> mats;
};