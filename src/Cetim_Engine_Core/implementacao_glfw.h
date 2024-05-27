#pragma once

#include "RecursosT.h"
#include "input.h"
#include "LoopPrincipal.h"

#include <chrono>
#include <thread>
#include <cmath>

#include "args.h"

#include "benchmark.h"

#define SDL_ENABLE_OLD_NAMES
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

SDL_Window *window;

vector<SDL_Event> event_list = {};

bool text_getted_this_frame = false;

SDL_Surface *toSDLSurface(shared_ptr<imagem> img)
{
	if (img->data.empty())
	{
		cerr << "Image data is empty!" << endl;
		return nullptr;
	}

	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	int depth = 32;				// Profundidade de cor: 32 bits para RGBA
	int pitch = img->res.x * 4; // Cada pixel tem 4 bytes (RGBA)

	SDL_Surface *surface = SDL_CreateSurfaceFrom(
		const_cast<unsigned char *>(img->data.data()), // Ponteiro para os dados de pixel
		img->res.x,									   // Largura da imagem
		img->res.y,									   // Altura da imagem
		depth,										   // Profundidade de cor
		SDL_PIXELFORMAT_RGBA8888);

	if (surface == nullptr)
	{
		cerr << "SDL_CreateRGBSurfaceFrom Error: " << SDL_GetError() << endl;
	}

	return surface;
}

class gerenciador_janela_sdl : public gerenciador_janela
{
private:
	bool is_full_sceen = false;

public:
	bool running = true;
	SDL_Event event;
	SDL_GLContext glContext;
	Tempo::Timer sw;

	gerenciador_janela_sdl() {}
	bool esta_janela_inteira() { return is_full_sceen; }
	void mudar_cursor(shared_ptr<imagem> cursor) {}

	void mudar_imagem_janela(shared_ptr<imagem> janela)
	{
		if (janela != NULL)
		{
			SDL_SetWindowIcon(window, toSDLSurface(janela));
		}
	}

	void mudar_pos_cursor(float pos_x, float pos_y) {}

	void mudar_res(float res_x, float res_y)
	{
		SDL_SetWindowSize(window, res_x, res_y);
	}

	void enable_cursor(bool enable)
	{
		if (enable)
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
			SDL_ShowCursor();
		}
		else
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_HideCursor();
		}
	}

	bool e_janela_cheia()
	{
		return is_full_sceen;
	}

	vec2 pegar_res()
	{
		int width, height;
		SDL_GetWindowSize(window, &width, &height);
		return vec2(width, height);
	}

	void iniciar()
	{

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		window = SDL_CreateWindow(pegar_nome_arquivo(argumentos[1]).c_str(), configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (window == nullptr)
		{
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		}

		glContext = SDL_GL_CreateContext(window);
		if (!glContext)
		{
			std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
			SDL_GL_DeleteContext(glContext);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		mudar_imagem_janela(ManuseioDados::carregar_Imagem("icon.svg"));

		for (function<void()> f : Iniciar_Render_Func)
		{
			f();
		}
	}

	void encerrar()
	{
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
	}

	Tempo::Timer deltaTimer;

	long double tempoPerdido = 0;
	void Reindenizar()
	{

		while (true)
		{
			float tempoTotal = deltaTimer.get() + tempoPerdido; // Inclui o tempo perdido anterior
			Tempo::deltaTime = static_cast<int>(tempoTotal / Tempo::time_step) * Tempo::time_step;

			if (Tempo::deltaTime < Tempo::time_step)
			{
				this_thread::sleep_for(chrono::microseconds((unsigned int)(Tempo::deltaTime - Tempo::time_step) / 1000));
				continue;
			}

			tempoPerdido = tempoTotal - Tempo::deltaTime; // Calcula o novo tempo perdido

			deltaTimer.clear();

			break;
		}

		for (function<void()> f : Antes_Render_Func)
		{
			f();
		}

		cena_objetos_selecionados->atualisar();

		cena_objetos_selecionados->atualisar_transforms();

		cena_objetos_selecionados->atualisar_Logica_Scripst();

		reindenizar_cenario();

		for (function<void()> f : Depois_Render_Func)
		{
			f();
		}
	}

	void loop()
	{

		iniciar();

		while (running && gerente_janela->fechar == false)
		{
			event_list.clear();

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					running = false;
				}
				text_getted_this_frame = false;
				event_list.push_back(event);
			}

			// tempo
			float t = sw.get();
			sw.clear();
			Tempo::tempUltFrameRender = Tempo::varTempRender;
			Tempo::varTempRender = t;

			Tempo::FPS = 1 / Tempo::varTempRender;
			Reindenizar();

			// Swap buffers
			SDL_GL_SwapWindow(window);
		}

		encerrar();
	}

	ivec2 pos_janela;
	void setar_tela_inteira_como(bool tela_cheia)
	{
		SDL_SetWindowFullscreen(window, tela_cheia);
		is_full_sceen = tela_cheia;
	}

	~gerenciador_janela_sdl()
	{
		encerrar();
	}
};

class input_manager_sdl : public input_manager
{
public:
	input_manager_sdl() {}
	string text_input;
	teclado keyboard_input;
	input_mouse mouse_input;
	vector<joystick> joysticks_input;
	vector<TOUCHES> touch_screen_input;
	vr_headset_input vr_input;

	bool backspace_last_frame = false;
	string get_text_input()
	{
		if (!text_getted_this_frame)
		{
			for (SDL_Event e : event_list)
			{
				if (e.type == SDL_TEXTINPUT)
				{
					text_input += e.text.text;
					break;
				}
			}
			text_getted_this_frame = true;
		}

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (!text_input.empty() && state[SDL_SCANCODE_BACKSPACE] == true && backspace_last_frame == false)
		{
			text_input.resize(text_input.size() - 1); // Reduz o tamanho da string em 1
		}
		backspace_last_frame = state[SDL_SCANCODE_BACKSPACE];

		return text_input;
	}
	void set_text_input(bool on)
	{
		if (on)
		{
			text_input = "";
			SDL_StartTextInput();
		}
		else
		{
			text_input = "";
			SDL_StopTextInput();
		}
	}

	input_mouse get_mouse_input()
	{
		mouse_input.movimentos["movement_x"] = 0;
		mouse_input.movimentos["movement_y"] = 0;

		float x, y;
		Uint32 mouseButtons = SDL_GetMouseState(&x, &y);
		mouse_input.botoes["left"] = mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT);
		mouse_input.botoes["right"] = mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		mouse_input.botoes["scroll_button"] = mouseButtons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
		for (SDL_Event e : event_list)
		{
			if (e.type == SDL_MOUSEWHEEL)
			{
				mouse_input.botoes["scroll"] = e.wheel.y;
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				int mouseX = e.motion.x;
				int mouseY = e.motion.y;
				int relX = e.motion.xrel;
				int relY = e.motion.yrel;
				mouse_input.movimentos["x"] = mouseX;
				mouse_input.movimentos["y"] = mouseY;

				int width, height;
				SDL_GetWindowSize(window, &width, &height);
				mouse_input.movimentos["movement_x"] = (float)relX / (float)width;
				mouse_input.movimentos["movement_y"] = (float)relY / (float)height;

				mouse_input.movimentos["normalized_x"] = (float)mouseX / (float)width;
				mouse_input.movimentos["normalized_y"] = (float)mouseY / (float)height;
			}
		}

		return mouse_input;
	}

	teclado get_keyboard_input()
	{
		// Verifica o estado das teclas

		const Uint8 *state = SDL_GetKeyboardState(NULL);

		string keys = "    abcdefghijklmnopqrstuvwxyz1234567890";

		for (unsigned int i = 4; i < 39; i++)
		{
			string is;
			is.push_back(keys.at(i));

			const unsigned int scancode = SDL_SCANCODE_A + i;
			if (state[i])
			{
				keyboard_input.teclas[is] = 1;
			}
			else
			{
				keyboard_input.teclas[is] = 0;
			}
		}
		if (state[SDL_SCANCODE_ESCAPE])
		{
			keyboard_input.teclas["escape"] = 1;
		}
		else
		{
			keyboard_input.teclas["escape"] = 0;
		}
		if (state[SDL_SCANCODE_TAB])
		{
			keyboard_input.teclas["tab"] = 1;
		}
		else
		{
			keyboard_input.teclas["tab"] = 0;
		}
		if (state[SDL_SCANCODE_SPACE])
		{
			keyboard_input.teclas["space"] = 1;
		}
		else
		{
			keyboard_input.teclas["space"] = 0;
		}
		if (state[SDL_SCANCODE_LSHIFT])
		{
			keyboard_input.teclas["shift"] = 1;
		}
		else
		{
			keyboard_input.teclas["shift"] = 0;
		}
		if (state[SDL_SCANCODE_KP_ENTER])
		{
			keyboard_input.teclas["enter"] = 1;
		}
		else
		{
			keyboard_input.teclas["enter"] = 0;
		}
		if (state[SDL_SCANCODE_DELETE])
		{
			keyboard_input.teclas["delete"] = 1;
		}
		else
		{
			keyboard_input.teclas["delete"] = 0;
		}

		return keyboard_input;
	}

	void print_button(SDL_Gamepad *gamepad, Uint8 button)
	{

		switch (button)
		{
		case SDL_GAMEPAD_BUTTON_SOUTH:
			SDL_SetGamepadLED(gamepad, 255, 0, 0);
			std::cout << "Botão SOUTH pressionado\n";
			break;
		case SDL_GAMEPAD_BUTTON_WEST:
			std::cout << "Botão WEST pressionado\n";
			SDL_SetGamepadLED(gamepad, 0, 0, 255);
			break;
		case SDL_GAMEPAD_BUTTON_NORTH:
			std::cout << "Botão NORTH pressionado\n";
			SDL_SetGamepadLED(gamepad, 0, 255, 0);
			break;
		case SDL_GAMEPAD_BUTTON_EAST:
			std::cout << "Botão EAST pressionado\n";
			SDL_SetGamepadLED(gamepad, 255, 255, 0);
			break;
		}
	}

	void print_axis(SDL_Gamepad *gamepad, Uint8 axis, Sint16 power)
	{
		float fpower = (float)power / 32767.0;
		if (fpower > 0.2 || fpower < -0.2)
		{
			switch (axis)
			{
			case SDL_GAMEPAD_AXIS_LEFTX:
				print("SDL_GAMEPAD_AXIS_LEFTX", fpower);
				break;
			case SDL_GAMEPAD_AXIS_LEFTY:
				print("SDL_GAMEPAD_AXIS_LEFTY", fpower);
				break;

			case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
				print("SDL_GAMEPAD_AXIS_RIGHT_TRIGGER", fpower);
				SDL_RumbleGamepad(gamepad, 0, 65535, 500);
				break;
			case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
				print("SDL_GAMEPAD_AXIS_LEFT_TRIGGER", fpower);
				SDL_RumbleGamepad(gamepad, 65535, 0, 500);
				break;
			}
		}
	}

	unsigned int wait_next_print;

	map<SDL_CameraDeviceID, SDL_Gamepad *> sdl_gamepads;
	unsigned char get_gamepad_no(SDL_Gamepad * gp){
		unsigned char i = 0;
		for(pair<SDL_CameraDeviceID, SDL_Gamepad *> p : sdl_gamepads){
			if(gp == p.second){
				return i;
			}
			i++;
		}
		return 0;
	}

	vector<joystick> get_joysticks_input()
	{

		for (SDL_Event event : event_list)
		{
			if (event.type == SDL_EVENT_GAMEPAD_ADDED)
			{

				sdl_gamepads[event.cdevice.which] = SDL_OpenGamepad(event.cdevice.which);
				if (sdl_gamepads[event.cdevice.which])
				{
					std::cout << "Gamepad conectado: " << SDL_GetGamepadName(sdl_gamepads[event.cdevice.which]) << std::endl;
				}
				SDL_SetGamepadSensorEnabled(sdl_gamepads[event.cdevice.which], SDL_SENSOR_GYRO, SDL_TRUE);
			}
			else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
			{
				print_button(sdl_gamepads[event.cdevice.which], event.gbutton.button);
			}
			else if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
			{
				print_axis(sdl_gamepads[event.cdevice.which], event.gaxis.axis, event.gaxis.value);
			}
			else if (event.type == SDL_EVENT_GAMEPAD_SENSOR_UPDATE && event.gsensor.sensor == SDL_SENSOR_GYRO)
			{
				wait_next_print++;
				if (wait_next_print >= 30)
				{
					float *data = event.gsensor.data;
					print("gyroscope: ", data[0], data[1], data[2]);
					wait_next_print = 0;
				}
			}
			else if (event.type == SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION)
			{
				SDL_SetGamepadLED(sdl_gamepads[event.cdevice.which], event.gtouchpad.x * 255, event.gtouchpad.y * 255, 0);
			}
			else if (event.type == SDL_EVENT_GAMEPAD_TOUCHPAD_UP)
			{
				SDL_SetGamepadLED(sdl_gamepads[event.cdevice.which], 0,0,0);
			}
			else if (event.type == SDL_EVENT_GAMEPAD_REMOVED)
			{
				SDL_Gamepad * gp = sdl_gamepads[event.cdevice.which];
				SDL_CloseGamepad(gp);
				sdl_gamepads.erase(event.cdevice.which);
				std::cout << "Gamepad desconectado\n";
			}
		}

		return {};
	}

	void set_led(unsigned char no, float r, float g, float b)
	{
		if (no < sdl_gamepads.size())
		{
			auto it = sdl_gamepads.begin();
			std::advance(it,no);
			SDL_SetGamepadLED(it->second, (unsigned char)(r / 255), (unsigned char)(g / 255), (unsigned char)(b / 255));
		}
	}

	void set_vibration(unsigned char no, float power_l, float power_r, float time)
	{
		if (no < sdl_gamepads.size())
		{
			auto it = sdl_gamepads.begin();
			std::advance(it,no);
			SDL_RumbleGamepad(it->second, power_l / 65535, power_r / 65535, time / 1000);
		}
	}

	TOUCHES get_touch_screen()
	{
		for (SDL_Event event : event_list)
		{
			if (event.type == SDL_EVENT_FINGER_DOWN)
			{
				wait_next_print++;
				if (wait_next_print >= 30)
				{
					print("toque");
					wait_next_print = 0;
				}
			}
			if (event.type == SDL_EVENT_FINGER_UP)
			{
				wait_next_print++;
				if (wait_next_print >= 30)
				{
					print("tfinger", event.tfinger.x, event.tfinger.y);
					wait_next_print = 0;
				}
			}
			if (event.type == SDL_EVENT_FINGER_MOTION)
			{
				wait_next_print++;
				if (wait_next_print >= 30)
				{
					wait_next_print = 0;
				}
			}
		}
		return {};
	}

	vr_headset_input get_vr_headset_input()
	{
		return vr_input;
	}

	void set_mouse_position(float x, float y) {}
};
