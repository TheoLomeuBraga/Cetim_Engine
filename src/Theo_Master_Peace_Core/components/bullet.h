#pragma once
#include <set>
#include "RecursosT.h"
#include "sceane.h"
#include "Tempo.h"
#include "game_object.h"

#include <btBulletDynamicsCommon.h>

/*
struct mesh {
    vector<vec3> vertices;
    vector<unsigned int> index
};
*/

class bullet : public componente
{
public:
    shared_ptr<malha> collision_mesh;
    char forma = caixa;
    float densidade = 1, atrito = 0;
	bool gatilho = false;
	char dinamica = estatico;
    vec3 escala = vec3(1,1,1);
	float escala_gravidade = 1;
	bool rotacionar = true;
    info_camada layer;
	vector<colis_info> colis_infos;
	vector<shared_ptr<objeto_jogo>> objects_colidinf;
    
    bullet() {}
    void iniciar() {}
    void atualisar() {}
    void colidir(colis_info col) {}
    void finalisar() {}
};

class bullet_charter : public componente
{
public:
    bullet_charter() {}
    void iniciar() {}
    void atualisar() {}
    void colidir(colis_info col) {}
    void finalisar() {}
};


void atualisar_global_bullet(){}

