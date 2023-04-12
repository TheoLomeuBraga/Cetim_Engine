#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
using namespace std;

#include "Config.h"
#include "Tempo.h"
#include "vibrar_controle.cpp"
#include "RecursosT.h"

class input_manager
{
public:
	input_manager() {}
	//virtual void loop() {}

	virtual string get_text_input() {return "";}
	virtual void set_text_input(bool on) {}

	virtual teclado get_keyboard_input(){return teclado();}
	virtual input_mouse get_mouse_input() {return input_mouse();}
	virtual vector<joystick> get_joysticks_input(){vector<joystick> vj; return vj;}

	virtual vec2 set_mouse_position(float x,float y){return vec2(0,0);}
};

//input_mouse Mouse;
//teclado Teclado;
//bool conectado;
//vector<joystick> Joystick;

input_manager *manuseio_inputs;

/*
void loop_input()
{

	while (manuseio_inputs == NULL)
	{
		escrever("esperando input_manager");
	}
	escrever("iniciando input_manager");
	manuseio_inputs->loop();
}
*/

//#define MOUSE Mouse
//#define TECLADO Teclado
//#define JOYSTICK Joystick
