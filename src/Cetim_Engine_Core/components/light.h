#pragma once

#include <iostream>
#include <functional>
using String = std::string;
#include "RecursosT.h"
#include "game_object.h"

enum tipo_luz {
	ponto = 0,
	holofote = 1,
	direcional = 2,
	global = 3
};

class fonte_luz : public componente {
public:

	unsigned char tipo_luz;
	vec3 cor;
	float angulo_alcance;
	float distancia;

	bool ligado = true;
	fonte_luz() {}
};