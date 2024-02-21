#pragma once

#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourNavMeshBuilder.h"
#include "DetourTileCache.h"

#include <iostream>
#include <functional>
using String = std::string;

#include "RecursosT.h"
#include "game_object.h"
#include "transform.h"

class poly_mesh : public componente
{
public:

	bool ligado = true;
	bool usar_oclusao = true;
	float porcentagem_pode_ocupar_tela = 1;
	uint8_t camada = 0;
	vector<shared_ptr<malha>> malhas;
	vector<Material> mats;
	
	vector<shared_ptr<transform_>> transforms;

	poly_mesh() {}
	~poly_mesh() {}
};


