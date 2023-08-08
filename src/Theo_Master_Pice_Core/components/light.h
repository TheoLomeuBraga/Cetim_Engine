#pragma once

#include <iostream>
#include <functional>
using namespace std;
#include "RecursosT.h"
#include "game_object.h"

enum tipo_luz {
	ponto = 0,
	holofote = 1,
	direcional = 2,//tipo um sol
	global = 3
};

class fonte_luz : public componente {
public:

	char tipo_luz;

	vec3 direcao = vec3(0,0,-1);
	vec3 cor;
	float angulo;
	float alcance;

	bool ligado = true;
	fonte_luz() {}
};

