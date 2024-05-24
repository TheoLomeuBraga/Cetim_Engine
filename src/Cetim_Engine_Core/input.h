#pragma once
#include <thread>
using String = std::string;

#include "Config.h"
#include "Tempo.h"
// #include "vibrar_controle.cpp"
#include "RecursosT.h"

#define TOUCHES std::vector<vec2>

class input_manager
{
public:
	input_manager() {}
	// virtual void loop() {}
	string text_input;
	teclado keyboard_input;
	input_mouse mouse_input;
	vector<joystick> joysticks_input;
	vector<TOUCHES> touch_screen_input;
	vr_headset_input vr_input;

	virtual string get_text_input() { return ""; }
	virtual void set_text_input(bool on) {}

	virtual teclado get_keyboard_input() { return teclado(); }
	virtual input_mouse get_mouse_input() { return input_mouse(); }
	virtual TOUCHES get_touch_screen() { return {}; }
	
	virtual vector<joystick> get_joysticks_input()
	{
		vector<joystick> vj;
		return vj;
	}
	virtual void set_led(unsigned char no,unsigned char r,unsigned char g,unsigned char b){}
	virtual void set_vibration(unsigned char no,float power_l,float power_r,float time){}
	virtual vr_headset_input get_vr_headset_input(){
		return vr_input;
	}

	virtual void set_mouse_position(float x, float y) {}
};

// input_mouse Mouse;
// teclado Teclado;
// bool conectado;
// vector<joystick> Joystick;

input_manager *manuseio_inputs;

void get_input()
{
	if (manuseio_inputs != NULL)
	{
		manuseio_inputs->get_text_input();
		manuseio_inputs->get_keyboard_input();
		manuseio_inputs->get_mouse_input();
		manuseio_inputs->get_joysticks_input();
		manuseio_inputs->get_vr_headset_input();
	}
}

void get_input_using_threads()
{
	if (manuseio_inputs != NULL)
	{
		thread t1(&input_manager::get_text_input, manuseio_inputs);
		thread t2(&input_manager::get_keyboard_input, manuseio_inputs);
		thread t3(&input_manager::get_mouse_input, manuseio_inputs);
		thread t4(&input_manager::get_joysticks_input, manuseio_inputs);

		t1.join();
		t2.join();
		t3.join();
		t4.join();
	}
}

void start_get_input_loop()
{
	while (true)
	{
		if (manuseio_inputs != NULL)
		{
			manuseio_inputs->get_text_input();
			manuseio_inputs->get_keyboard_input();
			manuseio_inputs->get_mouse_input();
			manuseio_inputs->get_joysticks_input();
		}
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

// #define MOUSE Mouse
// #define TECLADO Teclado
// #define JOYSTICK Joystick
