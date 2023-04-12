#include "RecursosT.h"
#include "input.h"
#include "LoopPrincipal.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

GLFWwindow *janela;

bool interromper_loop_input = false;

namespace mouse
{

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

		// Set up the scroll callback and retrieve scroll offset

		mouseInfo["scroll"] = static_cast<float>(offsety);
		return mouseInfo;
	}

	std::unordered_map<std::string, int> generateMouseMap(GLFWwindow *window)
	{
		std::unordered_map<std::string, int> mouseMap;

		for (int button = GLFW_MOUSE_BUTTON_1; button <= GLFW_MOUSE_BUTTON_LAST; ++button)
		{
			int buttonState = glfwGetMouseButton(window, button);

			if (buttonState == GLFW_PRESS)
			{
				mouseMap[std::to_string(button)] = 2;
			}
			else if (buttonState == GLFW_RELEASE)
			{
				mouseMap[std::to_string(button)] = 3;
			}
			else
			{
				mouseMap[std::to_string(button)] = 0;
			}
		}

		return mouseMap;
	}

}

namespace teclas
{

	std::unordered_map<std::string, int> generateKeyboardMap(GLFWwindow *window)
	{
		std::unordered_map<std::string, int> keyboardMap;

		for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
		{
			int keyState = glfwGetKey(window, key);

			std::string keyIdentifier;
			/*
			const char *keyName = glfwGetKeyName(key, 0);
			

			
			if (keyName != nullptr)
			{
				keyIdentifier = keyName;
			}
			else
			{
				//keyIdentifier = "Key_" + std::to_string(key);
				keyIdentifier = std::to_string(key);
			}
			*/

			keyIdentifier = std::to_string(key);
			
			if (keyState == GLFW_PRESS)
			{
				keyboardMap[keyIdentifier] = 2;
			}
			else if (keyState == GLFW_RELEASE)
			{
				keyboardMap[keyIdentifier] = 3;
			}
			else
			{
				keyboardMap[keyIdentifier] = 0;
			}
		}

		return keyboardMap;
	}

	std::string inputText;

	bool delet_last_frame = false;

	bool read_input_text = false;

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
				++count;
			}
		}

		return count;
	}

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
					joystickKeyMap[std::to_string(i)] = 1;
				}
				else if (buttons[i] == GLFW_PRESS)
				{
					joystickKeyMap[std::to_string(i)] = 2;
				}
				else if (buttons[i] == GLFW_RELEASE && prevJoystickButtonsState[i] == GLFW_PRESS)
				{
					joystickKeyMap[std::to_string(i)] = 3;
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
				joystickAxes["axis_" + std::to_string(i)] = axes[i];
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

	/*

	Teclado.teclas = teclas::generateKeyboardMap(janela);

	teclas::read_input_text = Teclado.pegar_input_texto;
	Teclado.input_texto = teclas::getTextInput();

	Mouse.botoes = mouse::generateMouseMap(janela);
	Mouse.movimentos = mouse::generateMouseInfo(janela); // < corrigir
	
	
	int joystick_size = controle::countConnectedJoysticks();
	Joystick.resize(joystick_size);
	
	for (int i = 0; i < joystick_size; i++)
	{

		Joystick[i].botoes = controle::generateJoystickKeyMap(i);
		Joystick[i].eixos = controle::generateJoystickAxes(i);

	}
	
	*/

	Tempo::varInputTemp = Tempo::tempo - Tempo::tempUltFrame;
	Tempo::tempUltFrame = Tempo::tempo;
}

void inicioInput()
{

	while (janela == NULL)
	{
	}

	glfwSetScrollCallback(janela, mouse::scroll_callback);

	teclas::initializeTextInput(janela);

	cout << "Inputs Online" << endl;

	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(janela, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	loopInput();
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
	
	string get_text_input() {return "";}
	void set_text_input(bool on) {}

	teclado get_keyboard_input(){return teclado();}
	input_mouse get_mouse_input() {return input_mouse();}
	vector<joystick> get_joysticks_input(){vector<joystick> vj; return vj;}

	vec2 set_mouse_position(float x,float y){return vec2(0,0);}
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

void mudar_posicao_cursor(float x, float y)
{
	glfwSetCursorPos(janela, x, y);
}

void mudar_imagem_cursor(shared_ptr<imagem> img)
{
	if (img == NULL)
	{
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		GLFWimage image;
		image.width = img->res.x;
		image.height = img->res.y;
		image.pixels = &img->data[0];

		glfwSetCursor(janela, glfwCreateCursor(&image, 0, 0));
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

	// mudar_render_res(x, y);
	glfwSetWindowSize(janela, x, y);
	// configuracoes::janelaConfig.X = x;
	// configuracoes::janelaConfig.Y = y;
	cout << "tamanho da janela mudado para: " << x << " " << y << endl;
	int a, b;
	glfwGetWindowSize(janela, &a, &b);
	cout << "resolução da janela " << a << " " << b << endl;
}

bool janelaInteira = false;

void IniciarJanela()
{

	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		cout << "falha en iniciar o GLFW\n";
	}

	glfwWindowHint(GLFW_SAMPLES, configuracoes::janelaConfig.antiCerrilhado); // 4x antialiasing
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
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(janela); // Initialize GLEW
	glewExperimental = true;		// Needed in core profile
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
}

void loop_janela()
{

	// tempo
	Tempo::varTempRender = Tempo::tempUltFrameRender - Tempo::tempo;
	Tempo::varTempRender = Tempo::tempo - Tempo::tempUltFrameRender;
	Tempo::tempUltFrameRender = Tempo::tempo;
	Tempo::FPS = 1 / Tempo::varTempRender;

	glfwPollEvents();

	Reindenizar();

	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(janela);
	glfwPollEvents();
}

class gerenciador_janela_glfw : public gerenciador_janela
{
public:
	gerenciador_janela_glfw(bool tela_inteira) { janela_inteira = tela_inteira; }
	bool esta_janela_inteira() { return !glfwGetWindowAttrib(janela, GLFW_MAXIMIZED); }
	void mudar_cursor(shared_ptr<imagem> cursor) { mudar_imagem_cursor(cursor); }
	void mudar_imagem_janela(shared_ptr<imagem> janela) { mudar_logo_janela(janela); }
	void mudar_pos_cursor(float pos_x, float pos_y) { mudar_posicao_cursor(pos_x, pos_y); }
	void mudar_res(float res_x, float res_y) { MudarRes(res_x, res_y); }
	bool e_janela_cheia() { return janela_inteira; }
	vec2 pegar_res()
	{
		int x, y;
		glfwGetWindowSize(janela, &x, &y);
		// cout << "resolução da janela " << x << " " << y << endl;
		return vec2(x, y);
	}

	void loop()
	{
		fechar = glfwWindowShouldClose(janela) == 1;
		loop_janela();
	}
	void iniciar()
	{
		interromper_loop_input = false;
		IniciarJanela();
	}
	void encerrar()
	{
		interromper_loop_input = true;
		this_thread::sleep_for(chrono::milliseconds(100));
		glfwTerminate();
	}
	ivec2 pos_janela;
	void setar_tela_inteira_como(bool tela_cheia)
	{
		vec2 res = pegar_res();

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
};
