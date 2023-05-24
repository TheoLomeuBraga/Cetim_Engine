#pragma once

#include <math.h>
#include <regex>
#include <utility>
#include <array>
#include "RecursosT.h"
#include "ManusearArquivos.h"
#include "Config.h"
#include "input.h"
#include "Tempo.h"
#include "scene.h"
#include "LoopPrincipal.h"


#include <GL/glew.h>




#include "game_object.h"
#include "components/camera.h"
#include "components/transform.h"

struct mesh_ogl_struct
{
	int tamanho_indice;
	unsigned int vbo;
	unsigned int malha_buffer;
};
typedef struct mesh_ogl_struct mesh_ogl;

// EDITOR_MODE

string nome_shader_vert = "recursos/Shaders/vert/Shader.vert", nome_shader_geom = "recursos/Shaders/geom/Shader.geom", nome_shader_frag = "recursos/Shaders/frag/Shader.frag";

class OpenGL_API : public API_grafica_classe
{
public:
	bool show_oclusion_querie = false;

	int id_camera;

	unsigned int quad_array;

	unsigned int frame_buffer, deeph_buffer;
	vector<unsigned int> frame_buffers_texturas = vector<unsigned int>(SAIDAS_SHADER);

	map<fonte *, map<wchar_t, unsigned int>> fontes;
	map<fonte *, map<wchar_t, unsigned char *>> charters_bitmaps;

	map<imagem *, unsigned int> texturas;
	map<string, unsigned int> shaders, compute_shaders;
	map<malha *, mesh_ogl> malhas;

	// shader
	unsigned int ShaderGL;

	// variaveis
	unsigned int TF, VIS, PROJ;
	unsigned int tipo_vertice;

	bool limpar_cor, limpar_profundidade, usar_profundidade;

	unsigned int frame_buffer_editor;
	unsigned int deeph_buffer_editor;
	unsigned int frame_buffer_editor_texture;

	// https://www.mbsoftworks.sk/tutorials/opengl3/27-occlusion-query/

	map<shared_ptr<objeto_jogo>, unsigned int> oclusion_queries;
	map<shared_ptr<objeto_jogo>, int> oclusion_queries_resultados;

	unsigned int CompilarShader_ogl(vector<pair<string, unsigned int>> shader)
	{

		GLuint ProgramID = glCreateProgram();

		for (int i = 0; i < shader.size(); i++)
		{

			// Create the shaders
			GLuint VertexShaderID = glCreateShader(shader[i].second);

			// Read the Vertex Shader code from the file
			const char *vertex_file_path = shader[i].first.c_str();
			std::string VertexShaderCode;
			std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
			if (VertexShaderStream.is_open())
			{
				std::stringstream sstr;
				sstr << VertexShaderStream.rdbuf();
				VertexShaderCode = sstr.str();
				VertexShaderStream.close();
			}
			else
			{
				printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
				getchar();
				return 0;
			}

			GLint Result = GL_FALSE;
			int InfoLogLength;

			// Compile Vertex Shader
			char const *VertexSourcePointer = VertexShaderCode.c_str();
			glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
			glCompileShader(VertexShaderID);

			// Check Vertex Shader
			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0)
			{
				std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				printf("%s\n", &VertexShaderErrorMessage[0]);
			}

			// Link the program

			glAttachShader(ProgramID, VertexShaderID);

			// Check the program
			glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0)
			{
				std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
				glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
				printf("%s\n", &ProgramErrorMessage[0]);
			}
		}
		glLinkProgram(ProgramID);
		return ProgramID;
	}

	unsigned int pegar_shader(string shade)
	{
		if (shaders.find(shade) == shaders.end())
		{
			string nome = shade;
			vector<string> arquivos = {
				nome + string("/glsl.vert"),
				nome + string("/glsl.geom"),
				nome + string("/glsl.frag")};
			vector<pair<string, unsigned int>> p;

			if (ManuseioDados::Existe(arquivos[0]))
			{
				p.push_back(pair<string, unsigned int>(arquivos[0], GL_VERTEX_SHADER));
			}
			else
			{
				escrever("erro ao carregar shader " + nome);
			}

			if (ManuseioDados::Existe(arquivos[1]))
			{
				p.push_back(pair<string, unsigned int>(arquivos[1], GL_GEOMETRY_SHADER));
			}

			if (ManuseioDados::Existe(arquivos[2]))
			{
				p.push_back(pair<string, unsigned int>(arquivos[2], GL_FRAGMENT_SHADER));
			}
			else
			{
				escrever("erro ao carregar shader " + nome);
			}
			shaders.insert(pair<string, unsigned int>(shade, CompilarShader_ogl(p)));
		}

		return shaders[shade];
	}

	void remover_shader(string shade)
	{
		if (shaders.find(shade) != shaders.end())
		{
			glDeleteShader(shaders[shade]);
			shaders.erase(shade);
		}
	}

	unsigned int pegar_compute_shader(string shade)
	{
		if (compute_shaders.find(shade) == compute_shaders.end())
		{
			compute_shaders.insert(pair<string, unsigned int>(shade, CompilarShader_ogl({pair<string, unsigned int>(shade + "/glsl.comp", GL_COMPUTE_SHADER)})));
		}
		return compute_shaders[shade];
	}

	// https://medium.com/@daniel.coady/compute-shaders-in-opengl-4-3-d1c741998c03
	shared_ptr<imagem> rodar_compute_shader(Material mat, int resx, int resy, int chanels)
	{
		shared_ptr<imagem> ret = NULL;

		return ret;
	}

	void remover_compute_shader(string shade)
	{
		if (compute_shaders.find(shade) != compute_shaders.end())
		{
			glDeleteShader(compute_shaders[shade]);
			compute_shaders.erase(shade);
		}
	}

	shared_ptr<malha> oclusion_box;

	shared_ptr<transform_> teste_tf;
	shared_ptr<objeto_jogo> teste_cam = NULL;
	void iniciar_teste_tf_teste_cam()
	{
		teste_tf = make_shared<transform_>(transform_(false, vec3(10, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));
		teste_cam = novo_objeto_jogo();

		// teste_cam->adicionar_componente<camera>(camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 90, 1, 1, 0.01, 100));
		teste_cam->adicionar_componente<camera>(camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 10, 10, 0.01, 100));

		teste_cam->adicionar_componente<transform_>(transform_(false, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));
		teste_cam->pegar_componente<camera>()->paiTF = teste_cam->pegar_componente<transform_>().get();
		teste_cam->pegar_componente<camera>()->atualizar_tf();
	}

	void teste_desenhar_malha()
	{
		if (teste_cam == NULL)
		{
			iniciar_teste_tf_teste_cam();
		}
		teste_desenhar_malha(teste_tf, teste_cam);
	}
	void teste_desenhar_malha(shared_ptr<transform_> tf, shared_ptr<objeto_jogo> cam)
	{

		unsigned int shader_s = pegar_shader("resources/Shaders/teste_malha");
		glUseProgram(shader_s);

		// transform
		glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);
		glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &tf->matrizTransform[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &cam->pegar_componente<camera>()->matrizVisao[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &cam->pegar_componente<camera>()->matrizProjecao[0][0]);

		for (int i = 0; i < 4; i++)
		{
			glEnableVertexAttribArray(i);
		}

		// posição
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, posicao)));
		// uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, uv)));
		// normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, normal)));
		// cor
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, cor)));

		glDisable(GL_CULL_FACE);

		glDrawElements(
			GL_TRIANGLES,							   // mode
			malhas[oclusion_box.get()].tamanho_indice, // count
			GL_UNSIGNED_INT,						   // type
			(void *)0								   // element array buffer offset
		);
	}

	void criar_oclusion_querie(shared_ptr<objeto_jogo> obj)
	{
		if (oclusion_queries.find(obj) == oclusion_queries.end())
		{

			oclusion_queries.insert(pair<shared_ptr<objeto_jogo>, unsigned int>(obj, 0));
			oclusion_queries_resultados.insert(pair<shared_ptr<objeto_jogo>, unsigned int>(obj, 0));

			glGenQueries(1, &oclusion_queries[obj]);
		}
	}

	void rodar_oclusion_queries(shared_ptr<objeto_jogo> cam)
	{
		for (pair<shared_ptr<objeto_jogo>, unsigned int> p : oclusion_queries)
		{

			shared_ptr<transform_> tf = p.first->pegar_componente<transform_>();
			shared_ptr<render_malha> rm = p.first->pegar_componente<render_malha>();

			if (tf != NULL && rm != NULL && rm->usar_oclusao)
			{

				glBeginQuery(GL_SAMPLES_PASSED, p.second);
				unsigned int shader_s = pegar_shader("resources/Shaders/oclusion_querie");
				glUseProgram(shader_s);

				vec3 oq_sca(1, 1, 1);

				for (shared_ptr<malha> m : rm->malhas)
				{
					m->pegar_tamanho_maximo();
					oq_sca.x = std::max<float>(oq_sca.x, std::abs(m->tamanho_maximo.x));
					oq_sca.y = std::max<float>(oq_sca.y, std::abs(m->tamanho_maximo.y));
					oq_sca.z = std::max<float>(oq_sca.z, std::abs(m->tamanho_maximo.z));
				}

				mat4 oq_mat = tf->pegar_matriz();
				oq_mat = scale(oq_mat, oq_sca);

				glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &oq_mat[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &cam->pegar_componente<camera>()->matrizVisao[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &cam->pegar_componente<camera>()->matrizProjecao[0][0]);

				if (malhas.find(oclusion_box.get()) != malhas.end())
				{
					glDisable(GL_CULL_FACE);
					glDisable(GL_DEPTH_TEST);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, malhas[oclusion_box.get()].vbo);
					glBindBuffer(GL_ARRAY_BUFFER, malhas[oclusion_box.get()].malha_buffer);

					for (int i = 0; i < 4; i++)
					{
						glEnableVertexAttribArray(i);
					}

					// posição
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, posicao)));

					if (!show_oclusion_querie)
					{
						glColorMask(false, false, false, false);
					}
					glDrawElements(
						GL_TRIANGLES,							   // mode
						malhas[oclusion_box.get()].tamanho_indice, // count
						GL_UNSIGNED_INT,						   // type
						(void *)0								   // element array buffer offset
					);
					if (usar_profundidade)
					{
						glEnable(GL_DEPTH_TEST);
					}
					else
					{
						glDisable(GL_DEPTH_TEST);
					}
					glColorMask(true, true, true, true);
				}

				glEndQuery(GL_SAMPLES_PASSED);
			}
		}
	}

	void pegar_oclusion_queries()
	{
		for (pair<shared_ptr<objeto_jogo>, unsigned int> p : oclusion_queries)
		{
			glGetQueryObjectiv(p.second, GL_QUERY_RESULT, &oclusion_queries_resultados[p.first]);
			// cout << "oclusion querie result: " << oclusion_queries_resultados[p.first] << endl;
			if (p.first->pegar_componente<render_malha>()->usar_oclusao)
			{
				p.first->pegar_componente<render_malha>()->ligado = oclusion_queries_resultados[p.first] > 0;
			}
		}
	}

	void limpar_oclusion_queries()
	{
		map<shared_ptr<objeto_jogo>, unsigned int> oclusion_queries2;
		map<shared_ptr<objeto_jogo>, int> oclusion_queries_resultados2;
		for (pair<shared_ptr<objeto_jogo>, unsigned int> p : oclusion_queries)
		{
			if (p.first.use_count() > 3)
			{
				oclusion_queries2.insert(p);
				oclusion_queries_resultados2.insert(pair<shared_ptr<objeto_jogo>, int>(p.first, 0));
			}
			else
			{
				// cout << "querie: " << p.second << " foi deletada" << endl;
				glDeleteQueries(1, &p.second);
			}
		}
		oclusion_queries.swap(oclusion_queries2);
		oclusion_queries_resultados.swap(oclusion_queries_resultados2);
	}

	void aplicar_frame_buffer_principal()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void iniciar_lib()
	{

		modelo_gpu = string(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));

		vector<pair<string, unsigned int>> sha;
		sha.push_back(pair<string, unsigned int>(nome_shader_vert, GL_VERTEX_SHADER));
		sha.push_back(pair<string, unsigned int>(nome_shader_geom, GL_GEOMETRY_SHADER));
		sha.push_back(pair<string, unsigned int>(nome_shader_frag, GL_FRAGMENT_SHADER));

		oclusion_box = ManuseioDados::importar_obj("resources/3D Models/oclusion_box.obj")->malhas["Cube"];
		adicionar_malha(oclusion_box.get());

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		ShaderGL = pegar_shader("recursos/Shaders/padr�o");

		glUseProgram(ShaderGL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// glEnable(GL_CULL_FACE);

		TF = glGetUniformLocation(ShaderGL, "vert_mat.TF");
		VIS = glGetUniformLocation(ShaderGL, "vert_mat.VIS");
		PROJ = glGetUniformLocation(ShaderGL, "vert_mat.PROJ");
		tipo_vertice = glGetUniformLocation(ShaderGL, "vert_mat.tipo_vertice");

		glGenVertexArrays(1, &quad_array);
		glViewport(0, 0, configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y);

		// iniciar frame_buffers

		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		// deeph buffer
		glGenRenderbuffers(1, &deeph_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, deeph_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, deeph_buffer);
		// render texture
		for (int i = 0; i < SAIDAS_SHADER; i++)
		{
			glGenTextures(1, &frame_buffers_texturas[i]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, frame_buffers_texturas[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			if (pixel_perfeito)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frame_buffers_texturas[i], 0);
		}
		unsigned int attachments[10] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9};
		glDrawBuffers(SAIDAS_SHADER, attachments);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			escrever("framebuffer completo");
		}
		else
		{
			escrever("framebuffer incompleto");
		}

		if (EDITOR_MODE)
		{
			// frame
			glGenFramebuffers(1, &frame_buffer_editor);
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_editor);
			// deeph
			glGenRenderbuffers(1, &deeph_buffer_editor);
			glBindRenderbuffer(GL_RENDERBUFFER, deeph_buffer_editor);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, deeph_buffer_editor);
			// textura
			glGenTextures(1, &frame_buffer_editor_texture);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, frame_buffer_editor_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			if (pixel_perfeito)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_buffer_editor_texture, 0);
		}
	}

	OpenGL_API()
	{
		iniciar_lib();
	}

	void ogl_adicionar_textura(imagem *img)
	{

		if (texturas.find(img) == texturas.end() && img != NULL)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			texturas.insert(pair<imagem *, unsigned int>(img, 0));
			glGenTextures(1, &texturas[img]);
			glBindTexture(GL_TEXTURE_2D, texturas[img]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->res.x, img->res.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img->data[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	void remover_textura(imagem *img)
	{
		if (texturas.find(img) != texturas.end())
		{
			glDeleteTextures(1, &texturas[img]);
			texturas.erase(img);
		}
	}

	void adicionar_fonte(fonte *f)
	{
		/*
		if (fontes.find(f) == fontes.end()) {
			fontes.insert(pair<fonte*, unsigned int*>(f,new unsigned int[NUM_CARACTERES]));
			fontes_data.insert(pair<fonte*, vector<unsigned char*> >(f, vector<unsigned char*>()));
			fontes_data[f].resize(NUM_CARACTERES);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for (int i = 0; i < NUM_CARACTERES; i++) {

				glGenTextures(1, &fontes[f][i]);
				glBindTexture(GL_TEXTURE_2D, fontes[f][i]);

				//fontes_data[f][i] = vetor_ponteiro(f->Characters[i].data);
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, f->Characters[i].res.x, f->Characters[i].res.y, 0, GL_RED, GL_UNSIGNED_BYTE, fontes_data[f][i]);
				fontes_data[f][i] = f->chars[i].bitmap;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, f->chars[i].width, f->chars[i].height, 0, GL_RED, GL_UNSIGNED_BYTE, fontes_data[f][i]);


				//if (f->pixel_perfeito) {
				if (f->pixel_perfect) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
			}
		}
		*/

		if (fontes.find(f) == fontes.end())
		{

			fontes.insert(pair<fonte *, map<wchar_t, unsigned int>>(f, {}));
			charters_bitmaps.insert(pair<fonte *, map<wchar_t, unsigned char *>>(f, {}));

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (pair<wchar_t, caractere_info> char_fonte_atual : f->chars)
			{

				fontes[f].insert(pair<wchar_t, unsigned int>(char_fonte_atual.first, 0));

				glGenTextures(1, &fontes[f][char_fonte_atual.first]);
				glBindTexture(GL_TEXTURE_2D, fontes[f][char_fonte_atual.first]);

				charters_bitmaps[f][char_fonte_atual.first] = vetor_ponteiro<unsigned char>(f->chars[char_fonte_atual.first].bitmap);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, f->chars[char_fonte_atual.first].width, f->chars[char_fonte_atual.first].height, 0, GL_RED, GL_UNSIGNED_BYTE, charters_bitmaps[f][char_fonte_atual.first]);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				if (f->pixel_perfect)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
			}
		}
	}

	void remover_fonte(fonte *f)
	{
		/*
		if(fontes.find(f) != fontes.end())
		for (int i = 0; i < NUM_CARACTERES; i++) {
			glDeleteTextures(1, &fontes[f][i]);
			delete[] fontes[f];
			delete[] fontes_data[f][i];

			fontes.erase(f);
			fontes_data.erase(f);
		}
		map<fonte*,  map<wchar_t,unsigned char*>> charters_bitmaps
		*/
		for (pair<wchar_t, unsigned int> wc_ui : fontes[f])
		{
			glDeleteTextures(1, &wc_ui.second);
			fontes.erase(f);
		}
		for (pair<wchar_t, unsigned char *> bm : charters_bitmaps[f])
		{
			delete[] bm.second;
			charters_bitmaps.erase(f);
		}
	}

	// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
	// https://stackoverflow.com/questions/19081734/passing-custom-vertex-structure-to-glsl-shaders
	void adicionar_malha(malha *ma)
	{
		if (malhas.find(ma) == malhas.end())
		{
			mesh_ogl mo;
			malhas.insert(pair<malha *, mesh_ogl>(ma, mo));

			malhas[ma].tamanho_indice = ma->indice.size();

			// criar vbo
			glGenBuffers(1, &malhas[ma].vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, malhas[ma].vbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, ma->indice.size() * sizeof(unsigned int), &ma->indice[0], GL_STATIC_DRAW);

			// criar array vertices
			glGenBuffers(1, &malhas[ma].malha_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, malhas[ma].malha_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertice) * ma->vertices.size(), &ma->vertices[0], GL_STATIC_DRAW);

			glBindVertexArray(0);
		}
	}

	void selecionar_desenhar_malha(malha *ma, int tipo)
	{
		if (malhas.find(ma) != malhas.end())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, malhas[ma].vbo);
			glBindBuffer(GL_ARRAY_BUFFER, malhas[ma].malha_buffer);

			for (int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(i);
			}

			// posição
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, posicao)));
			// uv
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, uv)));
			// normal
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, normal)));
			// cor
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, cor)));

			glDrawElements(
				tipo,					   // mode
				malhas[ma].tamanho_indice, // count
				GL_UNSIGNED_INT,		   // type
				(void *)0				   // element array buffer offset
			);
		}
	}

	void remover_malha(malha *ma)
	{
		if (malhas.find(ma) != malhas.end())
		{
			glDeleteBuffers(1, &malhas[ma].vbo);
			glDeleteBuffers(1, &malhas[ma].malha_buffer);
			malhas.erase(ma);
		}
	}

	void mudar_render_res(int X, int Y)
	{
		glGenFramebuffers(1, &frame_buffer);
		glViewport(0, 0, X, Y);
	}

	shared_ptr<imagem> captura_de_tela()
	{

		unsigned char *data = new unsigned char[configuracoes::janelaConfig.X * configuracoes::janelaConfig.Y * 4];
		unsigned char *data2 = new unsigned char[configuracoes::janelaConfig.X * configuracoes::janelaConfig.Y * 4];

		glReadPixels(0, 0, configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, GL_RGBA, GL_UNSIGNED_BYTE, data2);

		for (int i = 0; i < configuracoes::janelaConfig.X; ++i)
		{
			for (int j = 0; j < configuracoes::janelaConfig.Y; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					data[(i + j * configuracoes::janelaConfig.X) * 4 + k] = data2[(i + (configuracoes::janelaConfig.Y - 1 - j) * configuracoes::janelaConfig.X) * 4 + k];
				}
			}
		}

		shared_ptr<imagem> ret = make_shared<imagem>(imagem(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, 4, data));

		delete[] data2;
		// ret->data = data;

		return ret;
	}

	bool checar_objeto_visivel(mat4 P, mat4 V, mat4 TF, vector<vec3> vertices_visao_render)
	{
		bool ret = true;
		vec3 min_max_tp[2];

		return ret;
	}

	void reindenizar_objeto(shared_ptr<objeto_jogo> obj, shared_ptr<objeto_jogo> cam)
	{

		if (obj->pegar_componente<transform_>() != NULL && cam->pegar_componente<transform_>() != NULL && cam->pegar_componente<camera>() != NULL)
		{
			shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
			shared_ptr<camera> ca = cam->pegar_componente<camera>();

			// render_shader
			shared_ptr<render_shader> rs = obj->pegar_componente<render_shader>();
			if (rs != NULL)
			{
				glDisable(GL_CULL_FACE);
				// shader
				unsigned int shader_s = pegar_shader(rs->mat.shad);
				glUseProgram(shader_s);

				glUniform1i(glGetUniformLocation(shader_s, "shedow_mode"), 0);

				// tempo
				glUniform1f(glGetUniformLocation(shader_s, "time"), Tempo::tempo);

				// texturas
				for (int i = 0; i < NO_TEXTURAS; i++)
				{
					if (rs->mat.texturas[i] != NULL)
					{
						ogl_adicionar_textura(rs->mat.texturas[i].get());
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, texturas[rs->mat.texturas[i].get()]);
						string nome_veriavel = "textures[";
						nome_veriavel + to_string(i) + "]";
						glUniform1i(glGetUniformLocation(shader_s, nome_veriavel.c_str()), i);
					}
				}

				// input
				for (int i = 0; i < NO_INPUTS; i++)
				{
					string nome_veriavel = string("inputs[") + to_string(i) + string("]");
					glUniform1i(glGetUniformLocation(shader_s, nome_veriavel.c_str()), rs->mat.inputs[i]);
				}

				// cor
				vec4 cor = rs->mat.cor;
				glUniform4f(glGetUniformLocation(shader_s, "color"), cor.x, cor.y, cor.z, cor.w);

				// uv
				vec4 uv = rs->mat.uv_pos_sca;
				glUniform4f(glGetUniformLocation(shader_s, "uv_position_scale"), uv.x, uv.y, uv.z, uv.w);

				// transform
				glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &tf->matrizTransform[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);

				// render
				glBindVertexArray(quad_array);
				glDrawArrays(GL_TRIANGLES, 0, rs->tamanho);
			}

			// render texto
			if (obj->pegar_componente<render_texto>() != NULL)
			{

				glDisable(GL_CULL_FACE);
				// https://learnopengl.com/In-Practice/Text-Rendering
				shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
				// shader
				unsigned int shader_s = pegar_shader(obj->pegar_componente<render_texto>()->mat.shad);
				glUseProgram(shader_s);

				glUniform1i(glGetUniformLocation(shader_s, "shedow_mode"), 0);

				// transform
				glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);

				glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);

				// tempo
				glUniform1f(glGetUniformLocation(shader_s, "tempo"), Tempo::tempo);

				// cor
				glUniform4f(glGetUniformLocation(shader_s, "color"),
							rt->mat.cor.x,
							rt->mat.cor.y,
							rt->mat.cor.z,
							rt->mat.cor.w);

				// uv
				glUniform4f(glGetUniformLocation(shader_s, "uv_position_scale"), 0, 0, 1, 1);

				glUniform1i(tipo_vertice, 1);
				glBindVertexArray(quad_array);

				std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
				const std::wregex ANSI_WREGEX(L"\\x1B\\[([0-9;]+)m", std::regex_constants::icase | std::regex_constants::ECMAScript);
				std::unordered_map<int, vec4> ANSI_colors;
				ANSI_colors[0] = vec4(1, 1, 1, 1);
				ANSI_colors[31] = vec4(1, 0, 0, 1);
				ANSI_colors[32] = vec4(0, 1, 0, 1);
				ANSI_colors[33] = vec4(1, 1, 0, 1);
				ANSI_colors[34] = vec4(0, 0, 1, 1);
				ANSI_colors[35] = vec4(1, 0, 1, 1);
				ANSI_colors[36] = vec4(0, 1, 1, 1);
				shared_ptr<fonte> font = rt->font;

				if (font != NULL)
				{
#define texto rt->texto
#define lugar_texto tf->matrizTransform

					vec2 pos_char(0, 0), pos_adi_char(0, 0), sca_char(1, 1);
					float altura_linha = 0;
					float tamanho_linha = 0;

					for (int i = 0; i < texto.size(); i++)
					{
						wchar_t letra = texto.at(i);
						if (letra == '\n')
						{
							altura_linha -= +rt->espaco_entre_linhas;
							pos_char.x = 0;
						}
						else if (letra == ' ')
						{
							pos_char.x += rt->tamanho_espaco;
							if (pos_char.x > rt->tamanho_max_linha)
							{
								altura_linha -= +rt->espaco_entre_linhas;
								pos_char.x = 0;
							}
						}
						else
						{

							caractere_info char_ = font->chars[letra];

							sca_char = vec2(char_.width, char_.height);

							vec2 bearing = vec2(char_.left, char_.top);

							if (rt->uniform_space_between_characters)
							{
								pos_adi_char = vec2(1, 0);
							}
							else
							{
								pos_adi_char = vec2((float)char_.adivancement / font->quality, 0);
							}

							pos_char.x += pos_adi_char.x;
							mat4 lugar_letra = translate(lugar_texto, vec3(pos_char.x, altura_linha + (((float)sca_char.y / (float)font->quality)), 0));
							lugar_letra = scale(lugar_letra, vec3(sca_char.x / font->quality, sca_char.y / font->quality, 1));

							// textura
							adicionar_fonte(font.get());
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, fontes[font.get()][letra]);

							glUniform1i(glGetUniformLocation(shader_s, "textures[0]"), 0);

							// transform
							glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &lugar_letra[0][0]);

							glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

							pos_char.x += pos_adi_char.x;

							if (pos_char.x > rt->tamanho_max_linha)
							{
								altura_linha -= +rt->espaco_entre_linhas;
								pos_char.x = 0;
							}
						}
					}
				}
			}

			// render tile map
			shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
			if (rtm != NULL && rtm->ligado && rtm->map_info != NULL && rtm->tiles != NULL)
			{

				glDisable(GL_CULL_FACE);

				// shader
				unsigned int shader_s = pegar_shader(rtm->mat.shad);
				glUseProgram(shader_s);

				glUniform1i(glGetUniformLocation(shader_s, "shedow_mode"), 0);

				// tempo
				glUniform1f(glGetUniformLocation(shader_s, "tempo"), Tempo::tempo);

				// transform

				glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);

				// textura
				ogl_adicionar_textura(rtm->tiles->tiles_img.get());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texturas[rtm->tiles->tiles_img.get()]);
				glUniform1i(glGetUniformLocation(shader_s, "textures[0]"), 0);

				// cor
				glUniform4f(glGetUniformLocation(shader_s, "color"),
							rtm->mat.cor.x,
							rtm->mat.cor.z,
							rtm->mat.cor.y,
							rtm->mat.cor.w);

				vector<vec3> vertices_visao_render_tile_map = {
					vec3(0, 0, 0),
					vec3(1, 0, 0),
					vec3(0, 1, 0),
					vec3(1, 1, 0)};

				// para cada camada

				if (rtm->apenas_camada == -1)
				{

					for (int a = 0; a < rtm->map_info->info.size(); a++)
					{

						// para cada tile X
						for (int b = 0; b < rtm->map_info->res.x; b++)
						{

							// otimizar
							bool visivel = true;
							ivec3 local_tile_selecionado = vec3(b, 0, 0) * vec3(2, 0, 0);
							mat4 mat_tile = translate(tf->matrizTransform, (vec3)local_tile_selecionado);
							vec3 pos_tela = ca->matrizProjecao * ca->matrizVisao * mat_tile * vec4(0, 0, 0, 1);

							float tacha_erro = 1.5f;
							if (pos_tela.x < -tacha_erro || pos_tela.x > tacha_erro)
							{
								visivel = false;
							}

							if (visivel)
							{
								// para cada tile Y
								for (int c = 0; c < rtm->map_info->res.y; c++)
								{
									int tile_id = rtm->map_info->info[a][(c * rtm->map_info->res.x) + b];

									if (tile_id != 0)
									{
										ivec3 local_tile_selecionado = vec3(b, c, a);
										mat4 mat_tile = translate(tf->matrizTransform, (vec3)local_tile_selecionado * vec3(2, -2, -0.001));
										ivec2 quant_t = rtm->tiles->quant_tiles;
										ivec2 tile_selecionado((tile_id % quant_t.x) - 1, (float)(int)tile_id / quant_t.x);

										glUniform4f(glGetUniformLocation(shader_s, "uv_position_scale"),
													(float)(tile_selecionado.x) / quant_t.x,
													(float)(tile_selecionado.y) / quant_t.y,
													(float)1 / quant_t.x,
													(float)1 / quant_t.y);
										glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &mat_tile[0][0]);
										glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
									}
								}
							}
						}
					}
				}
				else
				{
					int a = rtm->apenas_camada;

					for (int b = 0; b < rtm->map_info->res.x; b++)
					{

						// otimizar
						bool visivel = true;
						ivec3 local_tile_selecionado = vec3(b, 0, 0) * vec3(2, 0, 0);
						mat4 mat_tile = translate(tf->matrizTransform, (vec3)local_tile_selecionado);
						vec3 pos_tela = ca->matrizProjecao * ca->matrizVisao * mat_tile * vec4(0, 0, 0, 1);

						float tacha_erro = 1.5f;
						if (pos_tela.x < -tacha_erro || pos_tela.x > tacha_erro)
						{
							visivel = false;
						}

						if (visivel)
						{
							// para cada tile Y
							for (int c = 0; c < rtm->map_info->res.y; c++)
							{
								int tile_id = rtm->map_info->info[a][(c * rtm->map_info->res.x) + b];

								if (tile_id != 0)
								{
									ivec3 local_tile_selecionado = vec3(b, c, a);
									mat4 mat_tile = translate(tf->matrizTransform, (vec3)local_tile_selecionado * vec3(2, -2, -0.001));
									ivec2 quant_t = rtm->tiles->quant_tiles;
									ivec2 tile_selecionado((tile_id % quant_t.x) - 1, (float)(int)tile_id / quant_t.x);

									glUniform4f(glGetUniformLocation(shader_s, "uv_position_scale"),
												(float)(tile_selecionado.x) / quant_t.x,
												(float)(tile_selecionado.y) / quant_t.y,
												(float)1 / quant_t.x,
												(float)1 / quant_t.y);
									glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &mat_tile[0][0]);
									glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
								}
							}
						}
					}
				}
			}

			// render sprite
			shared_ptr<render_sprite> RS = obj->pegar_componente<render_sprite>();
			if (RS != NULL && RS->tiles != NULL && RS->ligado)
			{

				glDisable(GL_CULL_FACE);

				// shader
				unsigned int shader_s = pegar_shader(RS->mat.shad);
				glUseProgram(shader_s);

				// tempo
				glUniform1f(glGetUniformLocation(shader_s, "time"), Tempo::tempo);

				// textura
				ogl_adicionar_textura(RS->tiles->tiles_img.get());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texturas[RS->tiles->tiles_img.get()]);
				glUniform1i(glGetUniformLocation(shader_s, "textures[0]"), 0);

				// cor
				vec4 cor = obj->pegar_componente<render_sprite>()->mat.cor;
				glUniform4f(glGetUniformLocation(shader_s, "color"), cor.x, cor.y, cor.z, cor.w);

				// uv
				int tile_id = RS->tile_selecionado;
				ivec2 quant_t = obj->pegar_componente<render_sprite>()->tiles->quant_tiles;
				ivec2 tile_selecionado((tile_id % quant_t.x), (float)(int)tile_id / quant_t.x);
				glUniform4f(glGetUniformLocation(shader_s, "uv_position_scale"),
							(float)(tile_selecionado.x) / quant_t.x,
							(float)(tile_selecionado.y) / quant_t.y,
							(float)1 / quant_t.x,
							(float)1 / quant_t.y);

				// transform
				glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);
				// glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &tf->matrizTransform[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &tf->matrizTransform[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);

				// render
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}

			// https://www.youtube.com/watch?v=LMpw7foANNA

			// render_malha

			shared_ptr<render_malha> RM = obj->pegar_componente<render_malha>();
			if (RM != NULL && RM->malhas.size() > 0 && RM->ligado && RM->malhas.size() > 0 && RM->mats.size() > 0)
			{
				criar_oclusion_querie(obj);

				for (int i = 0; i < std::min<float>((int)RM->mats.size(), (int)RM->malhas.size()); i++)
				{

					shared_ptr<malha> ma = RM->malhas[i];
					Material mat = RM->mats[i];
					if (malhas.find(ma.get()) == malhas.end())
					{
						adicionar_malha(ma.get());
					}
					if (ma != NULL)
					{
						// aplicar material
						unsigned int shader_s = pegar_shader(mat.shad);
						glUseProgram(shader_s);

						// tempo
						glUniform1f(glGetUniformLocation(shader_s, "time"), Tempo::tempo);

						// cor
						vec4 cor = RM->mats[i].cor;
						glUniform4f(glGetUniformLocation(shader_s, "color"), cor.x, cor.y, cor.z, cor.w);

						// pos_sca
						vec4 pos_sca = RM->mats[i].uv_pos_sca;
						glUniform4f(glGetUniformLocation(shader_s, "uv_position_scale"), pos_sca.x, pos_sca.y, pos_sca.z, pos_sca.w);

						// texturas
						for (int a = 0; a < NO_TEXTURAS; a++)
						{
							if (RM->mats[i].texturas[a] != NULL)
							{
								ogl_adicionar_textura(RM->mats[i].texturas[a].get());
								glActiveTexture(GL_TEXTURE0 + a);
								glBindTexture(GL_TEXTURE_2D, texturas[RM->mats[i].texturas[a].get()]);
								string nome_veriavel = "textures[";
								nome_veriavel + to_string(a) + "]";
								glUniform1i(glGetUniformLocation(shader_s, nome_veriavel.c_str()), a);
							}
						}

						// input
						for (int a = 0; a < NO_INPUTS; a++)
						{
							string nome_veriavel = "inputs[";
							nome_veriavel + to_string(a) + "]";
							glUniform1i(glGetUniformLocation(shader_s, nome_veriavel.c_str()), RM->mats[i].inputs[a]);
						}

						// reindenizar malha
						// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/

						
						//switch (RM->lado_render)
						switch (RM->mats[i].lado_render)
						{
						case lado_render_malha::both:
							glDisable(GL_CULL_FACE);
							break;

						case lado_render_malha::front:
							glEnable(GL_CULL_FACE);
							glCullFace(GL_BACK);
							break;

						case lado_render_malha::back:
							glEnable(GL_CULL_FACE);
							glCullFace(GL_FRONT);

							break;
						}

						// transform

						glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);
						glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &tf->matrizTransform[0][0]);
						glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
						glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);

						selecionar_desenhar_malha(ma.get(), GL_TRIANGLES);
					}
				}
			}
		}
	}

	void reindenizar_camada_objetos(vector<shared_ptr<objeto_jogo>> obj, shared_ptr<objeto_jogo> cam)
	{

		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

		if (limpar_cor)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			limpar_cor = false;
		}
		if (limpar_profundidade)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			limpar_profundidade = false;
		}
		if (usar_profundidade)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		glUniform1f(glGetUniformLocation(ShaderGL, "tempo"), Tempo::tempo);

		for (int i = 0; i < SAIDAS_SHADER; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, frame_buffers_texturas[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res_interna.x, res_interna.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frame_buffers_texturas[i], 0);
		}
		glViewport(0, 0, res_interna.x, res_interna.y);

		for (int i = 0; i < obj.size(); i++)
		{
			if (obj[i] > 0 && cam > 0)
			{

				reindenizar_objeto(obj[i], cam);
			}
		}
	}

	void desenhar_tela(Material mat)
	{

		glBindVertexArray(quad_array);

		glUniform1i(tipo_vertice, 2);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void limpar_bufers_render(bool cor, bool profundidade)
	{
		if (cor)
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		if (profundidade)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	void ogl_iniciar_frame()
	{
	}

	void ogl_aplicar_pos_processamento()
	{

		// aplicar resolu��o

		aplicar_frame_buffer_principal();
		vec2 res = loop_principal::pegar_res();
		glViewport(0, 0, res.x, res.y);

		// aplicar shader
		unsigned int pp_shader = pegar_shader(pos_processamento_info.shad);
		glUseProgram(pp_shader);
		// aplicar frame_buffers_texturas
		for (int i = 0; i < SAIDAS_SHADER; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, frame_buffers_texturas[i]);
			string local = string("post_procesing_render_input[") + to_string(i) + "]";
			glUniform1i(glGetUniformLocation(pp_shader, local.c_str()), i);
		}
		// alicar pos processamento
		glBindVertexArray(quad_array);
		glUniform1i(tipo_vertice, 2);

		glUniform4f(glGetUniformLocation(pp_shader, "color"), pos_processamento_info.cor.x, pos_processamento_info.cor.y, pos_processamento_info.cor.z, pos_processamento_info.cor.w);

		// tempo
		glUniform1f(glGetUniformLocation(pp_shader, "time"), Tempo::tempo);

		// texturas
		for (int i = 0; i < NO_TEXTURAS; i++)
		{
			if (pos_processamento_info.texturas[i] != NULL)
			{
				ogl_adicionar_textura(pos_processamento_info.texturas[i].get());
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, texturas[pos_processamento_info.texturas[i].get()]);
				string nome_veriavel = "textures[";
				nome_veriavel + to_string(i) + "]";
				glUniform1i(glGetUniformLocation(pp_shader, nome_veriavel.c_str()), i);
			}
		}

		// input
		for (int i = 0; i < NO_INPUTS; i++)
		{
			string nome_veriavel = "inputs[";
			nome_veriavel + to_string(i) + "]";
			glUniform1i(glGetUniformLocation(pp_shader, nome_veriavel.c_str()), pos_processamento_info.inputs[i]);
		}

		// cor
		vec4 cor = pos_processamento_info.cor;
		glUniform4f(glGetUniformLocation(pp_shader, "color"), cor.x, cor.y, cor.z, cor.w);

		// uv
		vec4 uv = pos_processamento_info.uv_pos_sca;
		glUniform4f(glGetUniformLocation(pp_shader, "uv_position_scale"), uv.x, uv.y, uv.z, uv.w);

		// glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual int pegar_id_obj(int X, int Y)
	{

		return 0;
	}

	void reindenizar_cenario()
	{

		pegar_oclusion_queries();
		limpar_oclusion_queries();

		// transparency

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		for (int a = 0; a < std::min<float>(info_render.size(), cena_objetos_selecionados->objetos_camadas_render.size()); a++)
		{

			if (info_render[a].iniciar_render)
			{
				ogl_iniciar_frame();
			}
			if (info_render[a].limpar_buffer_cores)
			{
				limpar_cor = true;
				glClear(GL_COLOR_BUFFER_BIT);
			}
			if (info_render[a].limpar_buffer_profundidade)
			{
				limpar_profundidade = true;
				glClear(GL_DEPTH_BUFFER_BIT);
			}

			if (info_render[a].usar_profundidade)
			{
				usar_profundidade = true;
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
			}
			else
			{
				usar_profundidade = false;
				glDisable(GL_DEPTH_TEST);
			}

			if (info_render[a].desenhar_objetos)
			{
				if (cena_objetos_selecionados->cameras.size() >= relevancia_camera + 1 && cena_objetos_selecionados->cameras[relevancia_camera] != NULL)
				{
					reindenizar_camada_objetos(cena_objetos_selecionados->objetos_camadas_render[a], cena_objetos_selecionados->cameras[relevancia_camera]);
				}
				else
				{
					escrever("erro camera faltando");
				}
			}

			rodar_oclusion_queries(cena_objetos_selecionados->cameras[relevancia_camera]);

			if (info_render[a].terminar_render)
			{
				ogl_aplicar_pos_processamento();
			}
		}
	}
};

// OpenGL_APP* APP;
