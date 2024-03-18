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
#include <future>

#include "game_object.h"
#include "camera.h"
#include "transform.h"
#include "poly_mesh.h"

#include "benchmark.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	unsigned int oclusion_querie_in_view = 0;

	string bone_matrixes_ids[256];

	Material material_last_frame;

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
				print("erro ao carregar shader de vertice " + nome);
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
				print("erro ao carregar shader de fragmento " + nome);
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
		teste_cam->adicionar_componente<camera>(camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 10, 10, 0.01, 100));
		teste_cam->adicionar_componente<transform_>(transform_(false, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));
		teste_cam->pegar_componente<camera>()->paiTF = teste_cam->pegar_componente<transform_>();
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

	void rodar_oclusion_queries(shared_ptr<objeto_jogo> cam, size_t camada)
	{

		glDisable(GL_CULL_FACE);
		unsigned int shader_s = pegar_shader("resources/Shaders/oclusion_querie");
		glUseProgram(shader_s);
		glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &cam->pegar_componente<camera>()->matrizVisao[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &cam->pegar_componente<camera>()->matrizProjecao[0][0]);

		if (!show_oclusion_querie)
		{
			glColorMask(false, false, false, false);
		}

		for (pair<shared_ptr<objeto_jogo>, unsigned int> p : oclusion_queries)
		{

			shared_ptr<transform_> tf = p.first->pegar_componente<transform_>();
			shared_ptr<render_malha> rm = p.first->pegar_componente<render_malha>();

			if (camada == rm->camada)
			{

				if (tf != NULL && rm != NULL && !tf->UI && rm->usar_oclusao && !tf->UI)
				{

					mat4 ajust = glm::scale(mat4(1.0), vec3(-1, 1, -1)) * tf->matrizTransform;

					glBeginQuery(GL_SAMPLES_PASSED, p.second);

					// ajustar
					glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);

					glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &ajust[0][0]);

					for (unsigned char i = 0; i < rm->malhas.size(); i++)
					{

						shared_ptr<malha> m = rm->malhas[i];

						if (rm->mats[i].cor.w < 1)
						{
							glDisable(GL_DEPTH_TEST);
						}
						else
						{
							glEnable(GL_DEPTH_TEST);
						}

						// glEnable(GL_DEPTH_TEST);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, malhas[m.get()].vbo);
						glBindBuffer(GL_ARRAY_BUFFER, malhas[m.get()].malha_buffer);

						// posição
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), reinterpret_cast<void *>(offsetof(vertice, posicao)));

						glDrawElements(
							GL_TRIANGLES,					// mode
							malhas[m.get()].tamanho_indice, // count
							GL_UNSIGNED_INT,				// type
							(void *)0						// element array buffer offset
						);
					}

					glEndQuery(GL_SAMPLES_PASSED);
				}
			}
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

		glColorMask(true, true, true, true);
	}

	void pegar_oclusion_queries()
	{
		for (pair<shared_ptr<objeto_jogo>, unsigned int> p : oclusion_queries)
		{
			glGetQueryObjectiv(p.second, GL_QUERY_RESULT, &oclusion_queries_resultados[p.first]);

			shared_ptr<render_malha> rm = p.first->pegar_componente<render_malha>();
			if (rm->usar_oclusao)
			{
				rm->ligado = oclusion_queries_resultados[p.first];
				if (oclusion_queries_resultados[p.first])
				{
					oclusion_querie_in_view++;
				}
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
				glDeleteQueries(1, &p.second);
			}
		}
		oclusion_queries.swap(oclusion_queries2);
		oclusion_queries_resultados.swap(oclusion_queries_resultados2);
		oclusion_querie_in_view = 0;
	}

	void aplicar_frame_buffer_principal()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void iniciar_lib()
	{

		modelo_gpu = string(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));

		for (size_t i = 0; i < 256; i++)
		{

			bone_matrixes_ids[i] = ("finalBonesMatrices[" + std::to_string(i) + "]");
		}

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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

				charters_bitmaps[f][char_fonte_atual.first] = vetor_ponteiro<unsigned char>(f->chars[char_fonte_atual.first].bitmap);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, f->chars[char_fonte_atual.first].width, f->chars[char_fonte_atual.first].height, 0, GL_RED, GL_UNSIGNED_BYTE, charters_bitmaps[f][char_fonte_atual.first]);

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

		for (pair<wchar_t, unsigned int> wc_ui : fontes[f])
		{
			glDeleteTextures(1, &wc_ui.second);
		}
		fontes.erase(f);

		for (pair<wchar_t, unsigned char *> bm : charters_bitmaps[f])
		{
			delete[] bm.second;
		}
		charters_bitmaps.erase(f);
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

			for (int i = 0; i < 6; i++)
			{
				glEnableVertexAttribArray(i);
			}

			// posição
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), (void *)(offsetof(vertice, posicao)));

			// uv
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertice), (void *)(offsetof(vertice, uv)));

			// normal
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), (void *)(offsetof(vertice, normal)));

			// cor
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), (void *)(offsetof(vertice, cor)));

			// boneIds
			glVertexAttribIPointer(4, 4, GL_INT, sizeof(vertice), (void *)(offsetof(vertice, id_ossos)));

			// weights
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(vertice), (void *)(offsetof(vertice, peso_ossos)));

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
		update_res = true;
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

	void apply_light(unsigned int shader_s)
	{

		size_t light_size = cena_objetos_selecionados->fontes_luzes_id.size();
		if (light_size)
		{
			print("lights on sceane: ", light_size);
		}
		glUniform1i(glGetUniformLocation(shader_s, "light_size"), light_size);

		for (size_t i = 0; i < light_size; i++)
		{
		}
	}

	void apply_transform(unsigned int shader_s, shared_ptr<transform_> tf, shared_ptr<camera> ca)
	{
		glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);

		if (!tf->UI)
		{
			mat4 ajust = glm::scale(mat4(1.0), vec3(-1, 1, -1)) * tf->matrizTransform;
			glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &ajust[0][0]);
		}
		else
		{
			glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &tf->matrizTransform[0][0]);
		}

		glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);
	}

	void apply_material(unsigned int shader_s, Material mat)
	{

		if (mat != material_last_frame)
		{

			glUniform1i(glGetUniformLocation(shader_s, "shedow_mode"), 0);
			glUniform1f(glGetUniformLocation(shader_s, "time"), Tempo::tempo);
			glUniform1f(glGetUniformLocation(shader_s, "softness"), mat.suave);
			glUniform1f(glGetUniformLocation(shader_s, "metallic"), mat.metalico);

			// texturas
			for (int i = 0; i < NO_TEXTURAS; i++)
			{
				if (mat.texturas[i] != NULL)
				{
					ogl_adicionar_textura(mat.texturas[i].get());
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, texturas[mat.texturas[i].get()]);

					if (mat.filtro[i] > 0)
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}
					else
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					}
					string nome_veriavel = string("textures[") + to_string(i) + string("]");
					glUniform1i(glGetUniformLocation(shader_s, nome_veriavel.c_str()), i);
				}
				else
				{
					break;
				}
			}

			switch (mat.lado_render)
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

			if (mat.cor.w == 1)
			{

				if (usar_profundidade)
				{
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LESS);
				}
			}
			else
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}

			for (pair<string, float> p : mat.inputs)
			{
				glUniform1f(glGetUniformLocation(shader_s, p.first.c_str()), p.second);
			}

			// cor
			glUniform4f(glGetUniformLocation(shader_s, "color"), mat.cor.x, mat.cor.y, mat.cor.z, mat.cor.w);

			// uv
			glUniform4f(glGetUniformLocation(shader_s, "uv_position_scale"), mat.uv_pos_sca.x, mat.uv_pos_sca.y, mat.uv_pos_sca.z, mat.uv_pos_sca.w);

			material_last_frame = mat;
		}
	}
	void reindenizar_objeto(shared_ptr<objeto_jogo> obj, shared_ptr<objeto_jogo> cam)
	{

		if (obj->pegar_componente<transform_>() != NULL && cam->pegar_componente<transform_>() != NULL && cam->pegar_componente<camera>() != NULL)
		{
			shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
			shared_ptr<camera> ca = cam->pegar_componente<camera>();

			if (tf->billboarding == 1)
			{
				tf->billboarding_planar(glm::vec3(cam->pegar_componente<transform_>()->pegar_matriz()[3]));
			}else if (tf->billboarding == 2){
				tf->billboarding_spherical(glm::vec3(cam->pegar_componente<transform_>()->pegar_matriz()[3]));
			}

			shared_ptr<render_malha> RM = obj->pegar_componente<render_malha>();
			if (RM != NULL && RM->malhas.size() > 0 && RM->mats.size() > 0)
			{

				bool is_skin = false;
				for (int i = 0; i < std::min<float>((int)RM->mats.size(), (int)RM->malhas.size()); i++)
				{
					if (RM->malhas[i]->pele)
					{
						is_skin = true;
						break;
					}
				}

				mat4 matrixes[256];

				if (is_skin)
				{
					// criar_oclusion_querie(obj);

					for (size_t i = 0; i < std::min(RM->bones.size(), (size_t)256); i++)
					{

						shared_ptr<transform_> bone_tf = RM->bones[i]->pegar_componente<transform_>();
						if (bone_tf != NULL)
						{
							matrixes[i] = glm::scale(mat4(1.0), vec3(-1, 1, -1)) * bone_tf->pegar_matriz() * bone_tf->offset_matrix;
						}
					}
				}
				else
				{
					criar_oclusion_querie(obj);
				}

				if (RM->ligado || is_skin)
				{

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

							// apply_transform(shader_s, tf, ca);
							// ajustar
							glUniform1i(glGetUniformLocation(shader_s, "ui"), tf->UI);

							mat4 ajust = tf->pegar_matriz();
							if (ma->pele)
							{
								ajust[3] = glm::vec4(glm::vec3(0, 0, 0), 1.0f);
							}
							ajust = glm::scale(mat4(1.0), vec3(-1, 1, -1)) * ajust;

							glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &ajust[0][0]);
							glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
							glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);

							apply_material(shader_s, mat);
							apply_light(shader_s);

							if (is_skin)
							{
								RM->usar_oclusao = false;
							}

							if (ma->pele)
							{

								// Benchmark_Timer bt("render_bone_malha");

								glUniform1i(glGetUniformLocation(shader_s, "skin_mode"), 1);

								for (size_t i = 0; i < std::min(RM->bones.size(), (size_t)256); i++)
								{

									glUniformMatrix4fv(glGetUniformLocation(shader_s, bone_matrixes_ids[i].c_str()), 1, GL_FALSE, &matrixes[i][0][0]);
								}
							}
							else
							{
								glUniform1i(glGetUniformLocation(shader_s, "skin_mode"), 0);
							}

							selecionar_desenhar_malha(ma.get(), GL_TRIANGLES);
						}
					}
				}
				return;
			}

			// render_shader
			shared_ptr<render_shader> rs = obj->pegar_componente<render_shader>();
			if (rs != NULL)
			{
				// shader
				unsigned int shader_s = pegar_shader(rs->mat.shad);
				glUseProgram(shader_s);

				apply_transform(shader_s, tf, ca);

				apply_material(shader_s, rs->mat);
				apply_light(shader_s);

				// render
				glDisable(GL_CULL_FACE);
				glBindVertexArray(quad_array);
				glDrawArrays(GL_TRIANGLES, 0, rs->tamanho);

				return;
			}

			// render texto
			shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
			if (rt != NULL && rt->font != NULL)
			{

				// https://learnopengl.com/In-Practice/Text-Rendering

				// shader
				unsigned int shader_s = pegar_shader(rt->mat.shad);
				glUseProgram(shader_s);

				// transform
				apply_transform(shader_s, tf, ca);

				apply_material(shader_s, rt->mat);

				glDisable(GL_CULL_FACE);

				glUniform1i(tipo_vertice, 1);
				glBindVertexArray(quad_array);

#define texto rt->texto

				unsigned int count_linhas = 0;
				for (int i = 0; i < texto.size(); i++)
				{
					if (texto.at(i) == '\n')
					{
						count_linhas++;
					}
				}

				shared_ptr<fonte> font = rt->font;

				if (font != NULL)
				{

					vec2 tamanho_texto = vec2(0, 0);
					vector<vec2> tamanho_linhas = {};
					if (rt->text_location_x != 0 && rt->text_location_y != 0)
					{
						tamanho_texto = rt->get_text_size();
						tamanho_linhas = rt->get_lines_size();
					}

					mat4 lugar_texto = tf->matrizTransform;

					vec2 pos_char(0, 0), pos_adi_char(0, 0), sca_char(1, 1);
					float altura_linha = 0;
					unsigned int no_linha = 0;
					float tamanho_linha = 0;

					for (int i = 0; i < texto.size(); i++)
					{
						wchar_t letra = texto.at(i);
						if (letra == '\n')
						{
							altura_linha -= +rt->espaco_entre_linhas;
							no_linha++;
							pos_char.x = 0;
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
								pos_adi_char = vec2((float)char_.adivancement / font->quality, 1 / font->quality);
							}

							pos_char.x += pos_adi_char.x;
							float size_char_y = ((((float)sca_char.y) / (float)font->quality));
							vec3 pos_letra = vec3(pos_char.x, altura_linha + size_char_y, 0);

							if (rt->text_location_x == render_text_location::CENTER)
							{
								pos_letra.x = (pos_letra.x - tamanho_texto.x) + (tamanho_texto.x - tamanho_linhas[no_linha].x);
							}
							else if (rt->text_location_x == render_text_location::LEFT)
							{

								pos_letra.x = pos_letra.x + ((-tamanho_texto.x - (tamanho_linhas[no_linha].x * 2)) + tamanho_texto.x);
							}

							if (rt->text_location_y == render_text_location::CENTER)
							{
								pos_letra.y = (pos_letra.y + (rt->espaco_entre_linhas / 2) * count_linhas) - size_char_y / 2;
							}
							else if (rt->text_location_y == render_text_location::TOP)
							{
								pos_letra.y = pos_letra.y - tamanho_texto.y;
							}
							else if (rt->text_location_y == render_text_location::DOWN)
							{
								pos_letra.y = pos_letra.y - rt->espaco_entre_linhas;
							}

							mat4 lugar_letra = translate(lugar_texto, pos_letra);
							lugar_letra = scale(lugar_letra, vec3(sca_char.x / font->quality, sca_char.y / font->quality, 1));

							// textura
							adicionar_fonte(font.get());
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, fontes[font.get()][letra]);

							glUniform1i(glGetUniformLocation(shader_s, "textures[0]"), 0);

							/*
							for (int i = 0; i < NO_INPUTS; i++)
							{
								string nome_veriavel = string("inputs[") + to_string(i) + string("]");
								glUniform1f(glGetUniformLocation(shader_s, nome_veriavel.c_str()), rt->mat.inputs[i]);
							}
							*/
							for (pair<string, float> p : rt->mat.inputs)
							{
								glUniform1f(glGetUniformLocation(shader_s, p.first.c_str()), p.second);
							}

							// transform
							glUniformMatrix4fv(glGetUniformLocation(shader_s, "transform"), 1, GL_FALSE, &lugar_letra[0][0]);

							glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

							pos_char.x += pos_adi_char.x;

							if (pos_char.x > rt->tamanho_max_linha)
							{
								altura_linha -= +rt->espaco_entre_linhas;
								no_linha++;
								pos_char.x = 0;
							}
						}
					}
				}

				return;
			}

			// render tile map
			shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
			if (rtm != NULL && rtm->ligado && rtm->map_info != NULL && rtm->tiles != NULL)
			{

				glDisable(GL_CULL_FACE);

				// shader
				unsigned int shader_s = pegar_shader(rtm->mat.shad);
				glUseProgram(shader_s);

				apply_material(shader_s, rtm->mat);

				// glUniform1i(glGetUniformLocation(shader_s, "shedow_mode"), 0);

				// tempo
				// glUniform1f(glGetUniformLocation(shader_s, "tempo"), Tempo::tempo);

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
							rtm->mat.cor.y,
							rtm->mat.cor.z,
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

							if (cam->pegar_componente<camera>()->ortografica)
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
											mat4 mat_tile = translate(tf->matrizTransform, (vec3)local_tile_selecionado * vec3(1, -1, 0));
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
							else
							{
								// para cada tile Y
								for (int c = 0; c < rtm->map_info->res.y; c++)
								{
									int tile_id = rtm->map_info->info[a][(c * rtm->map_info->res.x) + b];

									if (tile_id != 0)
									{
										ivec3 local_tile_selecionado = vec3(b, c, a);
										mat4 mat_tile = translate(tf->matrizTransform, ((vec3)local_tile_selecionado * vec3(1, -1, -0.001)) + vec3(0.5, -0.5, 0));
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
				return;
			}

			// render sprite
			shared_ptr<render_sprite> RS = obj->pegar_componente<render_sprite>();
			if (RS != NULL && RS->tiles != NULL && RS->ligado)
			{

				glDisable(GL_CULL_FACE);

				// shader
				unsigned int shader_s = pegar_shader(RS->mat.shad);
				glUseProgram(shader_s);
				apply_material(shader_s, RS->mat);

				// tempo
				glUniform1f(glGetUniformLocation(shader_s, "time"), Tempo::tempo);

				// textura
				ogl_adicionar_textura(RS->tiles->tiles_img.get());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texturas[RS->tiles->tiles_img.get()]);
				glUniform1i(glGetUniformLocation(shader_s, "textures[0]"), 0);
				if (RS->mat.filtro[0] > 0)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}

				// cor
				vec4 cor = obj->pegar_componente<render_sprite>()->mat.cor;
				// glUniform4f(glGetUniformLocation(shader_s, "color"), cor.x, cor.y, cor.z, cor.w);

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
				apply_transform(shader_s, tf, ca);

				// render
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				return;
			}

			// https://www.youtube.com/watch?v=LMpw7foANNA

			// render_malha
		}
		else if (obj->pegar_componente<poly_mesh>() && cam->pegar_componente<camera>() != NULL)
		{

			shared_ptr<poly_mesh> PMESH = obj->pegar_componente<poly_mesh>();
			shared_ptr<camera> ca = cam->pegar_componente<camera>();

			// criar_oclusion_querie(obj);

			if (PMESH->ligado)
			{

				for (int i = 0; i < std::min<float>((int)PMESH->mats.size(), (int)PMESH->malhas.size()); i++)
				{

					shared_ptr<malha> ma = PMESH->malhas[i];
					Material mat = PMESH->mats[i];
					if (malhas.find(ma.get()) == malhas.end())
					{
						adicionar_malha(ma.get());
					}
					if (ma != NULL)
					{

						// set up
						unsigned int shader_s = pegar_shader(mat.shad);
						glUseProgram(shader_s);

						glUniformMatrix4fv(glGetUniformLocation(shader_s, "vision"), 1, GL_FALSE, &ca->matrizVisao[0][0]);
						glUniformMatrix4fv(glGetUniformLocation(shader_s, "projection"), 1, GL_FALSE, &ca->matrizProjecao[0][0]);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, malhas[ma.get()].vbo);
						glBindBuffer(GL_ARRAY_BUFFER, malhas[ma.get()].malha_buffer);

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

						apply_material(shader_s, mat);
						apply_light(shader_s);

						unsigned int ui = glGetUniformLocation(shader_s, "ui"), transform = glGetUniformLocation(shader_s, "transform");

						for (shared_ptr<transform_> tf : PMESH->transforms)
						{

							mat4 ajust = glm::scale(mat4(1.0), vec3(-1, 1, -1)) * tf->matrizTransform;

							glUniform1i(ui, tf->UI);
							glUniformMatrix4fv(transform, 1, GL_FALSE, &ajust[0][0]);

							glDrawElements(
								GL_TRIANGLES,					 // mode
								malhas[ma.get()].tamanho_indice, // count
								GL_UNSIGNED_INT,				 // type
								(void *)0						 // element array buffer offset
							);
						}
					}
				}
			}
		}
	}

	bool is_transparent(Material mat)
	{
		return mat.cor.w < 1;
	}

	bool is_transparent(vector<Material> mats)
	{
		for (Material mat : mats)
		{
			if (mat.cor.w < 1)
			{
				return true;
			}
		}
		return false;
	}

	bool is_transparent(shared_ptr<objeto_jogo> obj)
	{
		/**/
		shared_ptr<poly_mesh> PMESH = obj->pegar_componente<poly_mesh>();
		if (PMESH != NULL && is_transparent(PMESH->mats))
		{
			return true;
		}

		shared_ptr<render_malha> RM = obj->pegar_componente<render_malha>();
		if (RM != NULL && is_transparent(RM->mats))
		{
			return true;
		}

		shared_ptr<render_sprite> RSP = obj->pegar_componente<render_sprite>();
		if (RSP != NULL && is_transparent(RSP->mat))
		{
			return true;
		}

		shared_ptr<render_tilemap> RTM = obj->pegar_componente<render_tilemap>();
		if (RTM != NULL && is_transparent(RTM->mat))
		{
			return true;
		}

		shared_ptr<render_texto> RT = obj->pegar_componente<render_texto>();
		if (RT != NULL && is_transparent(RT->mat))
		{
			return true;
		}

		shared_ptr<render_shader> RS = obj->pegar_componente<render_shader>();
		if (RS != NULL && is_transparent(RS->mat))
		{
			return true;
		}

		return false;
	}

	bool is_ui(shared_ptr<objeto_jogo> obj)
	{
		/**/
		shared_ptr<transform_> TF = obj->pegar_componente<transform_>();
		if (TF != NULL && TF->UI)
		{
			return true;
		}

		return false;
	}

	vector<vector<shared_ptr<objeto_jogo>>> separate_ordenate_objects(vector<shared_ptr<objeto_jogo>> objs, shared_ptr<objeto_jogo> cam)
	{
		/**/

		vector<shared_ptr<objeto_jogo>> transparent;
		vector<shared_ptr<objeto_jogo>> nontransparent;
		vector<shared_ptr<objeto_jogo>> ui;

		for (shared_ptr<objeto_jogo> obj : objs)
		{
			if (is_ui(obj))
			{
				ui.push_back(obj);
			}
			else if (is_transparent(obj))
			{
				transparent.push_back(obj);
			}
			else
			{
				nontransparent.push_back(obj);
			}
		}

		if (cam->pegar_componente<transform_>() != NULL)
		{
			transparent = tf_ordenate_by_distance(cam->pegar_componente<transform_>()->pegar_pos_global(), transparent);
			nontransparent = tf_ordenate_by_distance(cam->pegar_componente<transform_>()->pegar_pos_global(), nontransparent);
		}

		return {nontransparent, transparent, ui};

		// return {objs, {}};
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

		// glUniform1f(glGetUniformLocation(ShaderGL, "tempo"), Tempo::tempo);

		/**/
		if (update_res)
		{
			// delete
			glDeleteFramebuffers(1, &frame_buffer);
			glDeleteRenderbuffers(1, &deeph_buffer);
			for (int i = 0; i < SAIDAS_SHADER; i++)
			{
				glDeleteTextures(1, &frame_buffers_texturas[i]);
			}

			// create

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
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
			update_res = false;
		}

		for (int i = 0; i < SAIDAS_SHADER; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, frame_buffers_texturas[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res_interna.x, res_interna.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frame_buffers_texturas[i], 0);
		}
		glViewport(0, 0, res_interna.x, res_interna.y);

		vector<vector<shared_ptr<objeto_jogo>>> orderd_objects = separate_ordenate_objects(obj, cam);

		for (size_t a = 0; a < orderd_objects.size(); a++)
		{
			for (int i = 0; i < orderd_objects[a].size(); i++)
			{
				if (orderd_objects[a][i] > 0 && cam > 0)
				{
					if (is_transparent(orderd_objects[a][i]))
					{
						glDepthMask(GL_FALSE);
					}

					reindenizar_objeto(orderd_objects[a][i], cam);

					glDepthMask(GL_TRUE);
				}
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			string local = string("post_procesing_render_input[") + to_string(i) + string("]");
			glUniform1i(glGetUniformLocation(pp_shader, local.c_str()), i);
		}
		// alicar pos processamento
		glBindVertexArray(quad_array);
		glUniform1i(tipo_vertice, 2);

		// glUniform4f(glGetUniformLocation(pp_shader, "color"), pos_processamento_info.cor.x, pos_processamento_info.cor.y, pos_processamento_info.cor.z, pos_processamento_info.cor.w);

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
				string nome_veriavel = string("textures[") + to_string(i) + string("]");
				glUniform1i(glGetUniformLocation(pp_shader, nome_veriavel.c_str()), i);
			}
			else
			{
				break;
			}
		}

		// input
		/*
		for (int i = 0; i < NO_INPUTS; i++)
		{
			string nome_veriavel = string("inputs[") + to_string(i) + string("]");
			glUniform1i(glGetUniformLocation(pp_shader, nome_veriavel.c_str()), pos_processamento_info.inputs[i]);
		}
		*/

		apply_material(pp_shader, pos_processamento_info);

		// cor
		// vec4 cor = pos_processamento_info.cor;
		// glUniform4f(glGetUniformLocation(pp_shader, "color"), cor.x, cor.y, cor.z, cor.w);

		// uv
		// vec4 uv = pos_processamento_info.uv_pos_sca;
		// glUniform4f(glGetUniformLocation(pp_shader, "uv_position_scale"), uv.x, uv.y, uv.z, uv.w);

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

		// Benchmark_Timer bt("Render_Sceane");

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
					limpar_oclusion_queries();

					rodar_oclusion_queries(cena_objetos_selecionados->cameras[relevancia_camera], a);
					pegar_oclusion_queries();

					reindenizar_camada_objetos(cena_objetos_selecionados->objetos_camadas_render[a], cena_objetos_selecionados->cameras[relevancia_camera]);
				}
				else
				{
					escrever("erro camera faltando");
				}
			}
		}

		ogl_aplicar_pos_processamento();
	}

	~OpenGL_API()
	{
		print("OpenGL deleted");
	}
};

// OpenGL_APP* APP;
