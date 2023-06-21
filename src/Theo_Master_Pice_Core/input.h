#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
using namespace std;

#include "Config.h"
#include "Tempo.h"
//#include "vibrar_controle.cpp"
#include "RecursosT.h"

class input_manager
{
public:
	input_manager() {}
	//virtual void loop() {}
	string text_input;
	teclado keyboard_input;
	input_mouse mouse_input;
	vector<joystick> joysticks_input;

	virtual string get_text_input() {return "";}
	virtual void set_text_input(bool on) {}

	virtual teclado get_keyboard_input(){return teclado();}
	virtual input_mouse get_mouse_input() {return input_mouse();}
	virtual vector<joystick> get_joysticks_input(){vector<joystick> vj; return vj;}

	virtual void set_mouse_position(float x,float y){}
};



//input_mouse Mouse;
//teclado Teclado;
//bool conectado;
//vector<joystick> Joystick;

input_manager *manuseio_inputs;

void get_input(){
	if(manuseio_inputs != NULL){
		manuseio_inputs->get_text_input();
		manuseio_inputs->get_keyboard_input();
		manuseio_inputs->get_mouse_input();
		manuseio_inputs->get_joysticks_input();
	}
}

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