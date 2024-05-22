#pragma once
#include <iostream>
using String = std::string;
#include "RecursosT.h"


namespace configuracoes {


	bool desligar = false, minimizado = false, pausa = false;

	JanelaConfig janelaConfig = JanelaConfig("TMP", 256, 256, false, 0, false);

	

	void encerrar(){
		desligar = true;
	}








}






