#pragma once

#include <iostream>
#include <functional>
using namespace std;

#include "RecursosT.h"
#include "game_object.h"

class poly_mesh : public componente
{
public:

	bool ligado = true;
	bool usar_oclusao = true;
	float porcentagem_pode_ocupar_tela = 1;
	uint8_t camada = 0;
	vector<shared_ptr<malha>> malhas;
	vector<Material> mats;
	
	vector<shared_ptr<objeto_jogo>> objs;

	poly_mesh() {}
	~poly_mesh() {}
};


