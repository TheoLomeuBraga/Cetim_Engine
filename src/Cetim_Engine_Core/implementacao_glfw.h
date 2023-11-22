#pragma once

#include "RecursosT.h"
#include "input.h"
#include "LoopPrincipal.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "benchmark.h"

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
		pair<std::string, std::string>("0", "left"),
		pair<std::string, std::string>("1", "right"),
		pair<std::string, std::string>("2", "scroll_button"),
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

		pair<std::string, std::string>("48", "0"),
		pair<std::string, std::string>("49", "1"),
		pair<std::string, std::string>("50", "2"),
		pair<std::string, std::string>("51", "3"),
		pair<std::string, std::string>("52", "4"),
		pair<std::string, std::string>("53", "5"),
		pair<std::string, std::string>("54", "6"),
		pair<std::string, std::string>("55", "7"),
		pair<std::string, std::string>("56", "8"),
		pair<std::string, std::string>("57", "9"),

		pair<std::string, std::string>("32", "space"),

		pair<std::string, std::string>("65", "a"),
		pair<std::string, std::string>("66", "b"),
		pair<std::string, std::string>("67", "c"),
		pair<std::string, std::string>("68", "d"),
		pair<std::string, std::string>("69", "e"),
		pair<std::string, std::string>("70", "f"),
		pair<std::string, std::string>("71", "g"),
		pair<std::string, std::string>("72", "h"),
		pair<std::string, std::string>("73", "i"),
		pair<std::string, std::string>("74", "j"),
		pair<std::string, std::string>("75", "k"),
		pair<std::string, std::string>("76", "l"),
		pair<std::string, std::string>("77", "m"),
		pair<std::string, std::string>("78", "n"),
		pair<std::string, std::string>("79", "o"),
		pair<std::string, std::string>("80", "p"),
		pair<std::string, std::string>("81", "q"),
		pair<std::string, std::string>("82", "r"),
		pair<std::string, std::string>("83", "s"),
		pair<std::string, std::string>("84", "t"),
		pair<std::string, std::string>("85", "u"),
		pair<std::string, std::string>("86", "v"),
		pair<std::string, std::string>("87", "w"),
		pair<std::string, std::string>("88", "x"),
		pair<std::string, std::string>("89", "y"),
		pair<std::string, std::string>("90", "z"),

		pair<std::string, std::string>("92", "backslash"),
		pair<std::string, std::string>("256", "escape"),
		pair<std::string, std::string>("257", "enter"),
		pair<std::string, std::string>("258", "tab"),
		pair<std::string, std::string>("259", "backspace"),
		pair<std::string, std::string>("260", "insert"),
		pair<std::string, std::string>("261", "delete"),
		pair<std::string, std::string>("262", "right"),
		pair<std::string, std::string>("263", "left"),
		pair<std::string, std::string>("264", "down"),
		pair<std::string, std::string>("265", "up"),
		pair<std::string, std::string>("280", "caps_locl"),
		pair<std::string, std::string>("340", "shift"),
		pair<std::string, std::string>("341", "ctrl"),
		pair<std::string, std::string>("342", "alt"),
		pair<std::string, std::string>("344", "right_shift"),
		pair<std::string, std::string>("345", "right_ctrl"),
		pair<std::string, std::string>("346", "right_alt"),

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
			}else{
				break;
			}
		}

		return count;
	}

	#ifdef LINUX
	map<std::string, std::string> ajust_keys_map = {
		pair<std::string, std::string>("0", "a"),
		pair<std::string, std::string>("1", "b"),
		pair<std::string, std::string>("2", "x"),
		pair<std::string, std::string>("3", "y"),
		pair<std::string, std::string>("4", "lb"),
		pair<std::string, std::string>("5", "rb"),
		pair<std::string, std::string>("6", "back"),
		pair<std::string, std::string>("7", "start"),
		pair<std::string, std::string>("9", "la"),
		pair<std::string, std::string>("10", "ra"),
		pair<std::string, std::string>("11", "left"),
		pair<std::string, std::string>("12", "right"),
		pair<std::string, std::string>("13", "up"),
		pair<std::string, std::string>("14", "down"),

		pair<std::string, std::string>("axis_0", "lx"),
		pair<std::string, std::string>("axis_1", "ly"),
		pair<std::string, std::string>("axis_3", "rx"),
		pair<std::string, std::string>("axis_4", "ry"),
		pair<std::string, std::string>("axis_2", "lt"),
		pair<std::string, std::string>("axis_5", "rt"),
	};
	#endif

	#ifdef WINDOWS
	map<std::string, std::string> ajust_keys_map = {
		pair<std::string, std::string>("0", "a"),
		pair<std::string, std::string>("1", "b"),
		pair<std::string, std::string>("2", "x"),
		pair<std::string, std::string>("3", "y"),
		pair<std::string, std::string>("4", "lb"),
		pair<std::string, std::string>("5", "rb"),
		pair<std::string, std::string>("6", "back"),
		pair<std::string, std::string>("7", "start"),
		pair<std::string, std::string>("8", "la"),
		pair<std::string, std::string>("9", "ra"),
		pair<std::string, std::string>("10", "up"),
		pair<std::string, std::string>("11", "right"),
		pair<std::string, std::string>("12", "down"),
		pair<std::string, std::string>("13", "left"),

		pair<std::string, std::string>("axis_0", "lx"),
		pair<std::string, std::string>("axis_1", "ly"),
		pair<std::string, std::string>("axis_2", "rx"),
		pair<std::string, std::string>("axis_3", "ry"),
		pair<std::string, std::string>("axis_4", "lt"),
		pair<std::string, std::string>("axis_5", "rt"),
	};
	#endif
	

	static std::vector<int> prevJoystickButtonsState;

	std::unordered_map<std::string, int> generateJoystickKeyMap(int joystick)
	{
		std::unordered_map<std::string, int> joystickKeyMap;

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
		

		return joystickKeyMap;
	}

	std::unordered_map<std::string, float> generateJoystickAxes(int joystick)
	{
		std::unordered_map<std::string, float> joystickAxes;

			if (glfwJoystickPresent(joystick))
			{
				int axisCount;
				const float *axes = glfwGetJoystickAxes(joystick, &axisCount);

				for (int i = 0; i < axisCount; ++i)
				{
					#ifdef LINUX
					if (i == 2 || i == 5)
					{
						joystickAxes[ajust_keys_map[string("axis_") + std::to_string(i)]] = (axes[i] + 1) / 2;
					}
					else
					{
						joystickAxes[ajust_keys_map[string("axis_") + std::to_string(i)]] = axes[i];
					}
					#endif

					#ifdef WINDOWS
					if (i == 4 || i == 5)
					{
						joystickAxes[ajust_keys_map[string("axis_") + std::to_string(i)]] = (axes[i] + 1) / 2;
					}
					else
					{
						joystickAxes[ajust_keys_map[string("axis_") + std::to_string(i)]] = axes[i];
					}
					#endif
				}
			}
		

		return joystickAxes;
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
		vector<joystick> ji = {};
		ji.resize(controle::countConnectedJoysticks());
		for (int i = 0; i < joysticks_input.size(); i++)
		{
			ji[i].botoes = controle::generateJoystickKeyMap(i);
			ji[i].eixos = controle::generateJoystickAxes(i);
		}

		joysticks_input = ji;
		return joysticks_input;
	}

	void set_mouse_position(float x, float y) { mudar_posicao_cursor(x, y); }
};

// janela

bool janela_inteira = false;

void checarEstencoesGL()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
		exit(1);		   // or handle the error in a nicer way
	if (!GLEW_VERSION_2_1) // check that the machine supports the 2.1 API.
		exit(1);		   // or handle the error in a nicer way
}

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

void AntesReindenizar()
{
	for (function<void()> f : Iniciar_Render_Func)
	{
		f();
	}
}

bool iniciada_logica_scripts;

void Reindenizar()
{

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

	glfwWindowHint(GLFW_SAMPLES, configuracoes::janelaConfig.antiCerrilhado); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);							  // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	if (configuracoes::janelaConfig.transparente == true)
	{
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	}
	// icone

	GLFWimage *icon[1];

	// Open a window and create its OpenGL context

	if (janelaInteira)
	{
		// janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, configuracoes::janelaConfig.nome, glfwGetPrimaryMonitor(), NULL);
		janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, pegar_nome_arquivo(pegar_local_aplicacao()).c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		// janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, configuracoes::janelaConfig.nome, NULL, NULL);
		janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, pegar_nome_arquivo(pegar_local_aplicacao()).c_str(), NULL, NULL);
	}

	if (janela == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not opengl 3.3 compatible..\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(janela); // Initialize GLEW

	// glfwSwapInterval(1);

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "falha en iniciar o GLFW\n");
		cout << "falha en iniciar o GLFW\n";
	}

	glfwSetWindowSizeCallback(janela, MudarTamanhoJanela);
	// icone   load_icon("ico.png");

	mudar_logo_janela(ManuseioDados::carregar_Imagem("resources/ico.png"));

	AntesReindenizar();

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

	//Benchmark_Timer bt("window_loop");

	// tempo
	float t = sw.get();
	sw.clear();
	Tempo::tempUltFrameRender = Tempo::varTempRender;
	Tempo::varTempRender = t;
	
	
	//sw = Tempo::Timer();

	
	//float t = Tempo::tempo;
	//Tempo::varTempRender = (t - Tempo::tempUltFrameRender) * Tempo::velocidadeTempo;
	//Tempo::tempUltFrameRender = t;

	
	
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