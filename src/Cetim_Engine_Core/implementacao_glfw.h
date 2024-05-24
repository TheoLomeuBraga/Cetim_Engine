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

	TOUCHES get_touch_screen() { return {}; }

	void print_button(Uint8 button)
	{
		/*
		switch (button)
		{
		case SDL_GAMEPAD_BUTTON_LABEL_A:
			std::cout << "Botão A pressionado\n";
			break;
		case SDL_GAMEPAD_BUTTON_LABEL_B:
			std::cout << "Botão B pressionado\n";
			break;
		case SDL_GAMEPAD_BUTTON_LABEL_X:
			std::cout << "Botão X pressionado\n";
			break;
		case SDL_GAMEPAD_BUTTON_LABEL_Y:
			std::cout << "Botão Y pressionado\n";
			break;
		}
		*/

		switch (button)
		{
		case SDL_GAMEPAD_BUTTON_SOUTH:
			std::cout << "Botão SOUTH pressionado\n";
			break;
		case SDL_GAMEPAD_BUTTON_WEST:
			std::cout << "Botão WEST pressionado\n";
			break;
		case SDL_GAMEPAD_BUTTON_NORTH:
			std::cout << "Botão NORTH pressionado\n";
			break;
		case SDL_GAMEPAD_BUTTON_EAST:
			std::cout << "Botão EAST pressionado\n";
			break;
		}
	}

	void print_axis(Uint8 axis, Sint16 power)
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
			}
		}
	}

	unsigned int wait_next_print;
	SDL_Gamepad *gamepad = nullptr;
	vector<joystick> get_joysticks_input()
	{

		for (SDL_Event event : event_list)
		{
			if (event.type == SDL_EVENT_GAMEPAD_ADDED)
			{
				gamepad = SDL_OpenGamepad(event.cdevice.which);
				if (gamepad)
				{
					std::cout << "Gamepad conectado: " << SDL_GetGamepadName(gamepad) << std::endl;
				}
				SDL_SetGamepadSensorEnabled(gamepad, SDL_SENSOR_GYRO, SDL_TRUE);
			}
			else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
			{
				print_button(event.gbutton.button);
			}
			else if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
			{
				wait_next_print++;
				if (wait_next_print >= 30)
				{
					print_axis(event.gaxis.axis, event.gaxis.value);
					wait_next_print = 0;
				}
			}
			else if (event.type == SDL_EVENT_GAMEPAD_SENSOR_UPDATE && event.gsensor.sensor == SDL_SENSOR_GYRO)
			{
				wait_next_print++;
				if (wait_next_print >= 30)
				{
					float *data = event.gsensor.data;
					print("gyroscope: ",data[0], data[1], data[2]);
					wait_next_print = 0;
				}
			}
			else if (event.type == SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION)
			{
				wait_next_print++;
				if (wait_next_print >= 30)
				{
					print("touchpad: ",event.gtouchpad.x, event.gtouchpad.y);
					wait_next_print = 0;
				}
			}
			else if (event.type == SDL_EVENT_GAMEPAD_REMOVED)
			{
				SDL_CloseGamepad(gamepad);
				gamepad = nullptr;
				std::cout << "Gamepad desconectado\n";
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

// #define GLFW

#ifdef GLFW
#include <GLFW/glfw3.h>

GLFWwindow *janela;

bool interromper_loop_input = false;

namespace mouse
{

	vec2 mouse_movement_last_frame(0, 0);

	struct ScrollData
	{
		double xoffset;
		double yoffset;
	};

	double offsetx = 0;
	double offsety = 0;
	static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
	{
		ScrollData *scrollData = (ScrollData *)glfwGetWindowUserPointer(window);
		offsetx += xoffset;
		offsety += yoffset;
	}

	std::unordered_map<std::string, float> generateMouseInfo(GLFWwindow *window)
	{
		std::unordered_map<std::string, float> mouseInfo;
		double xpos, ypos;
		int screenWidth, screenHeight;

		glfwGetCursorPos(window, &xpos, &ypos);
		glfwGetWindowSize(window, &screenWidth, &screenHeight);

		float normalizedX = static_cast<float>(xpos) / screenWidth;
		float normalizedY = static_cast<float>(ypos) / screenHeight;

		mouseInfo["x"] = static_cast<float>(xpos);
		mouseInfo["y"] = static_cast<float>(ypos);
		mouseInfo["normalized_x"] = normalizedX;
		mouseInfo["normalized_y"] = normalizedY;

		mouseInfo["movement_x"] = normalizedX - mouse_movement_last_frame.x;
		mouseInfo["movement_y"] = normalizedY - mouse_movement_last_frame.y;

		mouse_movement_last_frame = vec2(normalizedX, normalizedY);

		// Set up the scroll callback and retrieve scroll offset

		mouseInfo["scroll"] = static_cast<float>(offsety);
		return mouseInfo;
	}

	map<std::string, std::string> ajust_keys_map = {
		std::pair<std::string, std::string>("0", "left"),
		std::pair<std::string, std::string>("1", "right"),
		std::pair<std::string, std::string>("2", "scroll_button"),
	};

	std::unordered_map<std::string, int> generateMouseMap(GLFWwindow *window)
	{
		std::unordered_map<std::string, int> mouseMap;

		for (int button = GLFW_MOUSE_BUTTON_1; button <= GLFW_MOUSE_BUTTON_LAST; ++button)
		{
			int buttonState = glfwGetMouseButton(window, button);

			if (buttonState == GLFW_PRESS)
			{
				mouseMap[ajust_keys_map[std::to_string(button)]] = 1;
			}
			else if (buttonState == GLFW_RELEASE)
			{
				mouseMap[ajust_keys_map[std::to_string(button)]] = 0;
			}
			else
			{
				mouseMap[ajust_keys_map[std::to_string(button)]] = 0;
			}
		}

		return mouseMap;
	}

}

namespace teclas
{

	std::string inputText;

	bool delet_last_frame = false;

	bool read_input_text = false;

	map<std::string, std::string> ajust_keys_map = {

		std::pair<std::string, std::string>("48", "0"),
		std::pair<std::string, std::string>("49", "1"),
		std::pair<std::string, std::string>("50", "2"),
		std::pair<std::string, std::string>("51", "3"),
		std::pair<std::string, std::string>("52", "4"),
		std::pair<std::string, std::string>("53", "5"),
		std::pair<std::string, std::string>("54", "6"),
		std::pair<std::string, std::string>("55", "7"),
		std::pair<std::string, std::string>("56", "8"),
		std::pair<std::string, std::string>("57", "9"),

		std::pair<std::string, std::string>("32", "space"),

		std::pair<std::string, std::string>("65", "a"),
		std::pair<std::string, std::string>("66", "b"),
		std::pair<std::string, std::string>("67", "c"),
		std::pair<std::string, std::string>("68", "d"),
		std::pair<std::string, std::string>("69", "e"),
		std::pair<std::string, std::string>("70", "f"),
		std::pair<std::string, std::string>("71", "g"),
		std::pair<std::string, std::string>("72", "h"),
		std::pair<std::string, std::string>("73", "i"),
		std::pair<std::string, std::string>("74", "j"),
		std::pair<std::string, std::string>("75", "k"),
		std::pair<std::string, std::string>("76", "l"),
		std::pair<std::string, std::string>("77", "m"),
		std::pair<std::string, std::string>("78", "n"),
		std::pair<std::string, std::string>("79", "o"),
		std::pair<std::string, std::string>("80", "p"),
		std::pair<std::string, std::string>("81", "q"),
		std::pair<std::string, std::string>("82", "r"),
		std::pair<std::string, std::string>("83", "s"),
		std::pair<std::string, std::string>("84", "t"),
		std::pair<std::string, std::string>("85", "u"),
		std::pair<std::string, std::string>("86", "v"),
		std::pair<std::string, std::string>("87", "w"),
		std::pair<std::string, std::string>("88", "x"),
		std::pair<std::string, std::string>("89", "y"),
		std::pair<std::string, std::string>("90", "z"),

		std::pair<std::string, std::string>("92", "backslash"),
		std::pair<std::string, std::string>("256", "escape"),
		std::pair<std::string, std::string>("257", "enter"),
		std::pair<std::string, std::string>("258", "tab"),
		std::pair<std::string, std::string>("259", "backspace"),
		std::pair<std::string, std::string>("260", "insert"),
		std::pair<std::string, std::string>("261", "delete"),
		std::pair<std::string, std::string>("262", "right"),
		std::pair<std::string, std::string>("263", "left"),
		std::pair<std::string, std::string>("264", "down"),
		std::pair<std::string, std::string>("265", "up"),
		std::pair<std::string, std::string>("280", "caps_locl"),
		std::pair<std::string, std::string>("340", "shift"),
		std::pair<std::string, std::string>("341", "ctrl"),
		std::pair<std::string, std::string>("342", "alt"),
		std::pair<std::string, std::string>("344", "right_shift"),
		std::pair<std::string, std::string>("345", "right_ctrl"),
		std::pair<std::string, std::string>("346", "right_alt"),

	};

	std::unordered_map<std::string, int> generateKeyboardMap(GLFWwindow *window)
	{
		std::unordered_map<std::string, int> keyboardMap;

		for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
		{
			int keyState = glfwGetKey(window, key);

			std::string keyIdentifier;

			keyIdentifier = ajust_keys_map[std::to_string(key)];

			if (keyState == GLFW_PRESS && !read_input_text)
			{
				keyboardMap[keyIdentifier] = 1;
			}
			else if (keyState == GLFW_RELEASE)
			{
				keyboardMap[keyIdentifier] = 0;
			}
			else if (key == GLFW_KEY_ENTER && keyState == GLFW_PRESS)
			{
				keyboardMap[keyIdentifier] = 1;
			}
			else
			{
				keyboardMap[keyIdentifier] = 0;
			}
		}

		return keyboardMap;
	}

	void charCallback(GLFWwindow *window, unsigned int codepoint)
	{
		inputText += static_cast<char>(codepoint);
	}

	void initializeTextInput(GLFWwindow *window)
	{
		glfwSetCharCallback(window, charCallback);
	}

	std::string getTextInput()
	{
		if (!read_input_text)
		{
			inputText = "";
			return inputText;
		}

		if (glfwGetKey(janela, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
		{
			if (!inputText.empty() && !delet_last_frame)
			{
				inputText.pop_back();
			}
			delet_last_frame = true;
		}
		else
		{
			delet_last_frame = false;
		}

		return inputText;
	}

}

namespace controle_xbox_360
{
#ifdef __unix__
	std::map<std::string, std::string> ajust_keys_map = {
		std::pair<std::string, std::string>("0", "a"),
		std::pair<std::string, std::string>("1", "b"),
		std::pair<std::string, std::string>("2", "x"),
		std::pair<std::string, std::string>("3", "y"),
		std::pair<std::string, std::string>("4", "lb"),
		std::pair<std::string, std::string>("5", "rb"),
		std::pair<std::string, std::string>("6", "back"),
		std::pair<std::string, std::string>("7", "start"),
		std::pair<std::string, std::string>("9", "la"),
		std::pair<std::string, std::string>("10", "ra"),
		std::pair<std::string, std::string>("11", "left"),
		std::pair<std::string, std::string>("12", "right"),
		std::pair<std::string, std::string>("13", "up"),
		std::pair<std::string, std::string>("14", "down"),

		std::pair<std::string, std::string>("axis_0", "lx"),
		std::pair<std::string, std::string>("axis_1", "ly"),
		std::pair<std::string, std::string>("axis_3", "rx"),
		std::pair<std::string, std::string>("axis_4", "ry"),
		std::pair<std::string, std::string>("axis_2", "lt"),
		std::pair<std::string, std::string>("axis_5", "rt"),
	};
#endif

#ifdef _WIN32
	std::map<std::string, std::string> ajust_keys_map = {
		std::pair<std::string, std::string>("0", "a"),
		std::pair<std::string, std::string>("1", "b"),
		std::pair<std::string, std::string>("2", "x"),
		std::pair<std::string, std::string>("3", "y"),
		std::pair<std::string, std::string>("4", "lb"),
		std::pair<std::string, std::string>("5", "rb"),
		std::pair<std::string, std::string>("6", "back"),
		std::pair<std::string, std::string>("7", "start"),
		std::pair<std::string, std::string>("8", "la"),
		std::pair<std::string, std::string>("9", "ra"),
		std::pair<std::string, std::string>("10", "up"),
		std::pair<std::string, std::string>("11", "right"),
		std::pair<std::string, std::string>("12", "down"),
		std::pair<std::string, std::string>("13", "left"),

		std::pair<std::string, std::string>("axis_0", "lx"),
		std::pair<std::string, std::string>("axis_1", "ly"),
		std::pair<std::string, std::string>("axis_2", "rx"),
		std::pair<std::string, std::string>("axis_3", "ry"),
		std::pair<std::string, std::string>("axis_4", "lt"),
		std::pair<std::string, std::string>("axis_5", "rt"),
	};
#endif

	static std::vector<float> prevJoystickButtonsState;

	unordered_map<string, float> generateJoystickKeyMap(int joystick)
	{
		std::unordered_map<std::string, float> joystickKeyMap;

		if (glfwJoystickPresent(joystick))
		{

			// print(glfwGetJoystickName(joystick));

			int buttonCount;
			const unsigned char *buttons = glfwGetJoystickButtons(joystick, &buttonCount);

			int axisCount;
			const float *axes = glfwGetJoystickAxes(joystick, &axisCount);

			if (prevJoystickButtonsState.empty())
			{
				prevJoystickButtonsState.resize(buttonCount, GLFW_RELEASE);
			}

			for (int i = 0; i < buttonCount; ++i)
			{
				if (ajust_keys_map.find(std::to_string(i)) != ajust_keys_map.end())
				{

					if (buttons[i] == GLFW_PRESS && prevJoystickButtonsState[i] == GLFW_RELEASE)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 1;
					}
					else if (buttons[i] == GLFW_PRESS)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 1;
					}
					else if (buttons[i] == GLFW_RELEASE && prevJoystickButtonsState[i] == GLFW_PRESS)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 0;
					}
					else
					{
						joystickKeyMap[std::to_string(i)] = 0;
					}

					prevJoystickButtonsState[i] = buttons[i];
				}
			}

			for (int i = 0; i < axisCount; ++i)
			{

				if (ajust_keys_map.find(string("axis_") + std::to_string(i)) != ajust_keys_map.end())
				{

#ifdef __linux__
					if (i == 2 || i == 5)
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = (axes[i] + 1) / 2;
					}
					else
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = axes[i];
					}
#endif

#ifdef _WIN32
					if (i == 4 || i == 5)
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = (axes[i] + 1) / 2;
					}
					else
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = axes[i];
					}
#endif
				}
			}
		}

		return joystickKeyMap;
	}

};

namespace controle_dualshock_4
{

#ifdef _WIN32
	std::map<std::string, std::string> ajust_keys_map = {
		std::pair<std::string, std::string>("0", "x"),
		std::pair<std::string, std::string>("1", "a"),
		std::pair<std::string, std::string>("2", "b"),
		std::pair<std::string, std::string>("3", "y"),
		std::pair<std::string, std::string>("4", "lb"),
		std::pair<std::string, std::string>("5", "rb"),

		std::pair<std::string, std::string>("8", "back"),
		std::pair<std::string, std::string>("9", "start"),
		std::pair<std::string, std::string>("10", "la"),
		std::pair<std::string, std::string>("11", "ra"),
		std::pair<std::string, std::string>("12", ""),
		std::pair<std::string, std::string>("13", ""),
		std::pair<std::string, std::string>("14", "up"),
		std::pair<std::string, std::string>("15", "right"),
		std::pair<std::string, std::string>("16", "down"),
		std::pair<std::string, std::string>("17", "left"),

		std::pair<std::string, std::string>("axis_0", "lx"),
		std::pair<std::string, std::string>("axis_1", "ly"),
		std::pair<std::string, std::string>("axis_2", "rx"),
		std::pair<std::string, std::string>("axis_3", "lt"),
		std::pair<std::string, std::string>("axis_4", "rt"),
		std::pair<std::string, std::string>("axis_5", "ry"),
	};
#endif

#ifdef __linux__
	std::map<std::string, std::string> ajust_keys_map = {
		std::pair<std::string, std::string>("0", "a"),
		std::pair<std::string, std::string>("1", "b"),
		std::pair<std::string, std::string>("2", "x"),
		std::pair<std::string, std::string>("3", "y"),
		std::pair<std::string, std::string>("4", "lb"),
		std::pair<std::string, std::string>("5", "rb"),

		std::pair<std::string, std::string>("8", "back"),
		std::pair<std::string, std::string>("9", "start"),
		std::pair<std::string, std::string>("10", ""),
		std::pair<std::string, std::string>("11", "la"),
		std::pair<std::string, std::string>("12", "ra"),
		std::pair<std::string, std::string>("13", "up"),
		std::pair<std::string, std::string>("14", "right"),
		std::pair<std::string, std::string>("15", "down"),
		std::pair<std::string, std::string>("16", "left"),

		std::pair<std::string, std::string>("axis_0", "lx"),
		std::pair<std::string, std::string>("axis_1", "ly"),
		std::pair<std::string, std::string>("axis_2", "lt"),
		std::pair<std::string, std::string>("axis_3", "rx"),
		std::pair<std::string, std::string>("axis_4", "ry"),
		std::pair<std::string, std::string>("axis_5", "rt"),
	};
#endif

	static std::vector<float> prevJoystickButtonsState;

	unordered_map<string, float> generateJoystickKeyMap(int joystick)
	{
		std::unordered_map<std::string, float> joystickKeyMap;

		if (glfwJoystickPresent(joystick))
		{

			int buttonCount;
			const unsigned char *buttons = glfwGetJoystickButtons(joystick, &buttonCount);

			if (prevJoystickButtonsState.empty())
			{
				prevJoystickButtonsState.resize(buttonCount, GLFW_RELEASE);
			}

			for (int i = 0; i < buttonCount; ++i)
			{

				// print("button",i,(int)buttons[i]);

				if (ajust_keys_map.find(std::to_string(i)) != ajust_keys_map.end())
				{

					if (buttons[i] == GLFW_PRESS && prevJoystickButtonsState[i] == GLFW_RELEASE)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 1;
					}
					else if (buttons[i] == GLFW_PRESS)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 1;
					}
					else if (buttons[i] == GLFW_RELEASE && prevJoystickButtonsState[i] == GLFW_PRESS)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 0;
					}
					else
					{
						joystickKeyMap[std::to_string(i)] = 0;
					}

					prevJoystickButtonsState[i] = buttons[i];
				}
			}

			int axisCount;
			const float *axes = glfwGetJoystickAxes(joystick, &axisCount);

			for (int i = 0; i < axisCount; ++i)
			{

				// print("axes",i,axes[i]);

				if (ajust_keys_map.find(string("axis_") + std::to_string(i)) != ajust_keys_map.end())
				{
#ifdef __linux__
					if (i == 2 || i == 5)
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = (axes[i] + 1) / 2;
					}
					else
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = axes[i];
					}
#endif
#ifdef _WIN32
					if (i == 3 || i == 4)
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = (axes[i] + 1) / 2;
					}
					else
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = axes[i];
					}
#endif
				}
			}
		}

		return joystickKeyMap;
	}

	static std::vector<float> prevJoystickButtonsStateTouch;

	unordered_map<string, float> generateJoystickKeyMapTouch(int joystick)
	{
		std::unordered_map<std::string, float> joystickKeyMap;

		if (glfwJoystickPresent(joystick))
		{
			print("{");
			int buttonCount;
			const unsigned char *buttons = glfwGetJoystickButtons(joystick, &buttonCount);

			if (prevJoystickButtonsStateTouch.empty())
			{
				prevJoystickButtonsStateTouch.resize(buttonCount, GLFW_RELEASE);
			}

			for (int i = 0; i < buttonCount; ++i)
			{

				print("		button:", (int)buttons[i]);

				if (ajust_keys_map.find(std::to_string(i)) != ajust_keys_map.end())
				{

					if (buttons[i] == GLFW_PRESS && prevJoystickButtonsStateTouch[i] == GLFW_RELEASE)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 1;
					}
					else if (buttons[i] == GLFW_PRESS)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 1;
					}
					else if (buttons[i] == GLFW_RELEASE && prevJoystickButtonsStateTouch[i] == GLFW_PRESS)
					{
						joystickKeyMap[ajust_keys_map[std::to_string(i)]] = 0;
					}
					else
					{
						joystickKeyMap[std::to_string(i)] = 0;
					}

					prevJoystickButtonsStateTouch[i] = buttons[i];
				}
			}

			int axisCount;
			const float *axes = glfwGetJoystickAxes(joystick, &axisCount);

			for (int i = 0; i < axisCount; ++i)
			{
				print("		axi:", axes[i]);
				if (ajust_keys_map.find(string("axis_") + std::to_string(i)) != ajust_keys_map.end())
				{
					if (i == 2 || i == 5)
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = (axes[i] + 1) / 2;
					}
					else
					{
						joystickKeyMap[ajust_keys_map[string("axis_") + std::to_string(i)]] = axes[i];
					}
				}
			}
			print("{");
		}

		return joystickKeyMap;
	}

};

namespace controle
{

	int countConnectedJoysticks()
	{
		int count = 0;

		for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i)
		{
			if (glfwJoystickPresent(i))
			{
				count++;
			}
			else
			{
				break;
			}
		}

		return count;
	}

	unordered_map<string, float> correct_mising_keys(unordered_map<string, float> mk)
	{
		unordered_map<string, float> ret = mk;

		unordered_map<string, float> base_reference = {
			std::pair<std::string, float>("a", 0),
			std::pair<std::string, float>("b", 0),
			std::pair<std::string, float>("x", 0),
			std::pair<std::string, float>("y", 0),
			std::pair<std::string, float>("lb", 0),
			std::pair<std::string, float>("rb", 0),
			std::pair<std::string, float>("back", 0),
			std::pair<std::string, float>("start", 0),
			std::pair<std::string, float>("la", 0),
			std::pair<std::string, float>("ra", 0),
			std::pair<std::string, float>("left", 0),
			std::pair<std::string, float>("right", 0),
			std::pair<std::string, float>("up", 0),
			std::pair<std::string, float>("down", 0),

			std::pair<std::string, float>("lx", 0),
			std::pair<std::string, float>("ly", 0),
			std::pair<std::string, float>("rx", 0),
			std::pair<std::string, float>("ry", 0),
			std::pair<std::string, float>("lt", 0),
			std::pair<std::string, float>("rt", 0),
		};

		for (pair<string, float> p : base_reference)
		{
			if (ret.find(p.first) == ret.end())
			{
				ret.insert(p);
			}
		}

		return ret;
	}

	unordered_map<string, float> last_inputs;

	unordered_map<string, float> generateJoystickKeyMap(int joystick)
	{

		std::string joystick_name = glfwGetJoystickName(joystick);
		// print(joystick_name);

		if (joystick_name == "Xbox 360 Wireless Receiver")
		{
			last_inputs = controle_xbox_360::generateJoystickKeyMap(joystick);
		}
		else if (joystick_name == "Sony Interactive Entertainment Wireless Controller" || joystick_name == "Wireless Controller")
		{
			last_inputs = controle_dualshock_4::generateJoystickKeyMap(joystick);
		}

		return correct_mising_keys(last_inputs);

		// defout
	}

}

void loopInput()
{

	while (interromper_loop_input)
	{
	}

	Tempo::varInputTemp = Tempo::tempo - Tempo::tempUltFrame;
	Tempo::tempUltFrame = Tempo::tempo;
}

void inicioInput()
{

	while (janela == NULL)
	{
	}

	loopInput();
}

void mudar_posicao_cursor(float x, float y)
{
	glfwSetCursorPos(janela, x, y);
}

class glfw_input_manager : public input_manager
{
public:
	glfw_input_manager() {}
	void loop()
	{
		inicioInput();
		escrever("glfw_input_manager online");
		while (configuracoes::desligar == false)
		{
			loopInput();
		}
	}

	string get_text_input()
	{
		text_input = teclas::getTextInput();
		return text_input;
	}
	void set_text_input(bool on) { teclas::read_input_text = on; }

	teclado get_keyboard_input()
	{
		teclado ki;
		ki.teclas = teclas::generateKeyboardMap(janela);
		ki.input_texto = get_text_input();
		keyboard_input = ki;
		return keyboard_input;
	}
	input_mouse get_mouse_input()
	{
		input_mouse mi;
		mi.movimentos = mouse::generateMouseInfo(janela);
		mi.botoes = mouse::generateMouseMap(janela);
		mouse_input = mi;
		return mouse_input;
	}
	vector<joystick> get_joysticks_input()
	{

		unsigned char joysticks_count = controle::countConnectedJoysticks();
		vector<joystick> ji = {};

		if (joysticks_count > 0)
		{

			ji.resize(joysticks_count);
			for (int i = 0; i < joysticks_input.size(); i++)
			{
				ji[i].inputs = controle::generateJoystickKeyMap(i);
			}
		}

		joysticks_input = ji;

		return joysticks_input;
	}

	void set_mouse_position(float x, float y) { mudar_posicao_cursor(x, y); }
};

// janela

bool janela_inteira = false;

void mudar_logo_janela(shared_ptr<imagem> img)
{
	if (img != NULL)
	{
		GLFWimage icones[2];
		icones[0].height = img->res.x;
		icones[0].width = img->res.y;
		icones[0].pixels = &img->data[0];
		icones[1].height = img->res.x;
		icones[1].width = img->res.y;
		icones[1].pixels = &img->data[0];
		glfwSetWindowIcon(janela, 2, icones);
	}
}

void mudar_imagem_cursor(shared_ptr<imagem> img)
{
	if (img == NULL)
	{
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		GLFWimage image;
		image.width = img->res.x;
		image.height = img->res.y;
		image.pixels = &img->data[0];

		GLFWcursor *customCursor = glfwCreateCursor(&image, 0, 0);

		glfwSetCursor(janela, customCursor);

		glfwDestroyCursor(customCursor);
	}
}

void ativar_cursor(bool ativar)
{
	if (ativar)
	{
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

vector<int> a;

bool iniciada_logica_scripts;

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

// Janela

void MudarTamanhoJanela(GLFWwindow *janela, int x, int y)
{
	// mudar_render_res(x, y);
}

void MudarRes(int x, int y)
{
	glfwSetWindowSize(janela, x, y);
	int a, b;
	glfwGetWindowSize(janela, &a, &b);
	configuracoes::janelaConfig.X = x;
	configuracoes::janelaConfig.Y = y;
}

bool janelaInteira = false;

bool checkGLES2Support()
{
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(100, 100, "Test", NULL, NULL);
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	// Check for OpenGL ES 2.0 support
	bool es2Supported = glewIsSupported("GL_ARB_ES2_compatibility") || glewIsSupported("GL_OES_EGL_image");
	glfwDestroyWindow(window);
	glfwTerminate();
	return es2Supported;
}

ivec2 monitor_res = ivec2(0, 0);
void IniciarJanela()
{

	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		cout << "falha en iniciar o GLFW\n";
	}

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	monitor_res.x = mode->width;
	monitor_res.y = mode->height;

	/*
	glfwWindowHint(GLFW_SAMPLES, configuracoes::janelaConfig.antiCerrilhado);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	// glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	if (configuracoes::janelaConfig.transparente == true)
	{
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	}
	// icone

	GLFWimage *icon[1];

	// Open a window and create its OpenGL context

	if (janelaInteira)
	{
		janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, pegar_nome_arquivo(argumentos[1]).c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, pegar_nome_arquivo(argumentos[1]).c_str(), NULL, NULL);
	}

	if (janela == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not OpenGL ES 3.0 compatible..\n");
		checkGLES2Support();
		glfwTerminate();
	}
	glfwMakeContextCurrent(janela); // Initialize GLEW
	glfwSwapInterval(0);

	// glfwSwapInterval(1);

	/**/
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "falha en iniciar o GLFW\n");
		cout << "falha en iniciar o GLFW\n";
	}

	glfwSetWindowSizeCallback(janela, MudarTamanhoJanela);

	shared_ptr<imagem> logo = ManuseioDados::carregar_Imagem("icon.png");
	if (logo)
	{
		mudar_logo_janela(logo);
	}
	else
	{
		logo = ManuseioDados::carregar_Imagem("icon.svg");
		if (logo)
		{
			mudar_logo_janela(logo);
		}
	}

	for (function<void()> f : Iniciar_Render_Func)
	{
		f();
	}

	cout << "Graficos Online" << endl;

	glfwSetInputMode(janela, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetScrollCallback(janela, mouse::scroll_callback);

	teclas::initializeTextInput(janela);

	cout << "Inputs Online" << endl;

	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(janela, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

Tempo::Timer sw;
void loop_janela()
{

	// Benchmark_Timer bt("window_loop");

	// tempo
	float t = sw.get();
	sw.clear();
	Tempo::tempUltFrameRender = Tempo::varTempRender;
	Tempo::varTempRender = t;

	Tempo::FPS = 1 / Tempo::varTempRender;
	Reindenizar();
	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(janela);
}

class gerenciador_janela_glfw : public gerenciador_janela
{
public:
	gerenciador_janela_glfw(bool tela_inteira) { janela_inteira = tela_inteira; }
	bool esta_janela_inteira() { return !glfwGetWindowAttrib(janela, GLFW_MAXIMIZED); }
	void mudar_cursor(shared_ptr<imagem> cursor) { mudar_imagem_cursor(cursor); }
	void enable_cursor(bool enable) { ativar_cursor(enable); }
	void mudar_imagem_janela(shared_ptr<imagem> janela) { mudar_logo_janela(janela); }
	void mudar_pos_cursor(float pos_x, float pos_y) { mudar_posicao_cursor(pos_x, pos_y); }
	void mudar_res(float res_x, float res_y) { MudarRes(res_x, res_y); }
	bool e_janela_cheia() { return janela_inteira; }
	vec2 pegar_res()
	{
		int x, y;
		glfwGetWindowSize(janela, &x, &y);
		return vec2(x, y);
	}

	void iniciar()
	{
		interromper_loop_input = false;
		IniciarJanela();
	}

	void encerrar()
	{
		setar_tela_inteira_como(false);

		interromper_loop_input = true;
		this_thread::sleep_for(chrono::milliseconds(100));
		glfwTerminate();
	}

	void loop()
	{

		iniciar();
		do
		{
			fechar = glfwWindowShouldClose(janela) == 1;
			loop_janela();
		} while (fechar == false);

		encerrar();
	}

	ivec2 pos_janela;
	void setar_tela_inteira_como(bool tela_cheia)
	{
		vec2 res = pegar_res();

		janela_inteira = tela_cheia;

		if (!janelaInteira)
		{
			glfwGetWindowPos(janela, &pos_janela.x, &pos_janela.y);
		}

		janelaInteira = tela_cheia;

		if (janelaInteira)
		{
			glfwSetWindowMonitor(janela, glfwGetPrimaryMonitor(), 500, 500, res.x, res.y, 0);
		}
		else
		{
			glfwSetWindowMonitor(janela, NULL, pos_janela.x, pos_janela.y, res.x, res.y, 0);
		}
	}

	~gerenciador_janela_glfw()
	{
		encerrar();
	}
};

#endif
