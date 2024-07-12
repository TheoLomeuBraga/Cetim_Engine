#pragma once
#include <thread>
using String = std::string;

#include "Config.h"
#include "Tempo.h"
// #include "vibrar_controle.cpp"
#include "RecursosT.h"

#define TOUCHES std::map<std::string,float>

typedef struct mobile_input_struct mobile_input;

class input_manager
{
public:
	input_manager() {}
	// virtual void loop() {}
	string text_input;
	teclado keyboard_input;
	input_mouse mouse_input;
	vector<joystick> joysticks_input;
	vr_headset_input vr_input;
	TOUCHES screen_touches;

	unsigned char main_controller_device = 0; // 0 = mouse & keyboard ; 1 = controller 

	virtual string get_text_input() { return ""; }
	virtual void set_text_input(bool on) {}

	virtual teclado get_keyboard_input() { return teclado(); }
	virtual input_mouse get_mouse_input() { return input_mouse(); }
	
	virtual joystick get_joysticks_input(unsigned char no)
	{
		joystick vj;
		return vj;
	}
	virtual TOUCHES get_screen_touches()
	{
		TOUCHES t;
		return t;
	}
	virtual void set_led(unsigned char no,float r,float g,float b){}
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
		manuseio_inputs->get_joysticks_input(0);
		manuseio_inputs->get_vr_headset_input();
		manuseio_inputs->get_screen_touches();
	}
}

void get_input_using_threads()
{
	if (manuseio_inputs != NULL)
	{
		thread t1(&input_manager::get_text_input, manuseio_inputs);
		thread t2(&input_manager::get_keyboard_input, manuseio_inputs);
		thread t3(&input_manager::get_mouse_input, manuseio_inputs);
		thread t4(&input_manager::get_joysticks_input, manuseio_inputs,0);
		thread t5(&input_manager::get_screen_touches, manuseio_inputs);

		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
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
			manuseio_inputs->get_joysticks_input(0);
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
