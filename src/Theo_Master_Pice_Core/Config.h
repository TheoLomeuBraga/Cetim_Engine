#pragma once
#include <iostream>
using namespace std;
#include "RecursosT.h"
#include <GLFW/glfw3.h>


namespace configuracoes {


	bool desligar = false, minimizado = false, pausa = false;

	JanelaConfig janelaConfig = JanelaConfig("TMP", 720, 720, false, 0, false);

	

	void encerrar(){
		desligar = true;
	}








}






