#pragma once
#include <set>
#include "RecursosT.h"
#include "sceane.h"
#include "Tempo.h"
#include "game_object.h"

class bullet : public componente
{
public:
    shared_ptr<objeto_jogo> esse_objeto = NULL;
    // bool iniciado = false;
    bullet() {}
    virtual void iniciar() {}
    virtual void atualisar() {}
    virtual void colidir(colis_info col) {}
    virtual void finalisar() {}
};

class bullet_charter : public componente
{
public:
    shared_ptr<objeto_jogo> esse_objeto = NULL;
    // bool iniciado = false;
    bullet_charter() {}
    virtual void iniciar() {}
    virtual void atualisar() {}
    virtual void colidir(colis_info col) {}
    virtual void finalisar() {}
};


void atualisar_global_bullet(){}