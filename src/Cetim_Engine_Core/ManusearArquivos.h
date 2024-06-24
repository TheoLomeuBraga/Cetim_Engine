#pragma once
#include <iostream>
using string = std::string;
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Converter_JSON.h"
#include "scene.h"
#include "table.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include "compression.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb-master/stb_image_write.h>
#include <stb-master/stb_image_resize.h>
#include <stb-master/stb_image.h>

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_ENABLE_DRACO
#include "tinygltf/tiny_gltf.h"

#include "read_map_file.h"

#define ANIMATION_FPS_COUNT 30
#include "read_gltf_file.h"

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

// fontes
#include <ft2build.h>
#include FT_FREETYPE_H

#include <thread>
#include <mutex>

namespace ManuseioDados
{

	int loading_requests_no = 0;

	std::string obterExtensaoArquivo(std::string nomeArquivo)
	{
		size_t pontoPos = nomeArquivo.find_last_of(".");
		if (pontoPos != std::string::npos && pontoPos < nomeArquivo.length() - 1)
		{
			return nomeArquivo.substr(pontoPos + 1);
		}
		return ""; // Retorna uma string vazia se não houver extensão
	}

	// Função para trocar a extensão de um arquivo a partir do nome do arquivo
	std::string trocarExtensaoArquivo(const std::string &nomeArquivo, const std::string &novaExtensao)
	{
		size_t pontoPos = nomeArquivo.find_last_of(".");
		if (pontoPos != std::string::npos && pontoPos < nomeArquivo.length() - 1)
		{
			return nomeArquivo.substr(0, pontoPos + 1) + novaExtensao;
		}
		return nomeArquivo + "." + novaExtensao; // Se não houver extensão, adiciona a nova extensão
	}

	std::set<std::string> loading_requests_files = {};

	std::mutex loading_request_mtx;

	bool has_loading_request(std::string file)
	{
		std::lock_guard<std::mutex> lock(loading_request_mtx);
		if (loading_requests_files.find(file) != loading_requests_files.end())
		{
			return true;
		}
		return false;
	}

	void add_loading_request(std::string file)
	{
		std::lock_guard<std::mutex> lock(loading_request_mtx);
		loading_requests_no += 1;
		loading_requests_files.insert(file);
	}

	void remove_loading_request(std::string file)
	{
		std::lock_guard<std::mutex> lock(loading_request_mtx);
		loading_requests_files.erase(file);
		loading_requests_no -= 1;
	}

	string diretorio;

	bool Existe(const char *nome)
	{
		ifstream file(nome);
		if (!file)
		{
			return false;
		}
		return true;
	}

	bool Existe(string nome)
	{
		return Existe(nome.c_str());
	}

	void Salvar(const char *nome, const char *conteudo)
	{

		ofstream file(nome);

		file << conteudo;

		file.close();
	}

	// template<typename X>
	string Carregar_string(string nome)
	{
		string s, s2;
		ifstream file(nome);
		const char *cc;

		if (file.is_open())
		{
			while (getline(file, s))
			{
				s2 += s + "\n";
			}
		}
		else
		{
			s2 = "erro";
		}

		return s2;
	}

	string Carregar_script(string nome)
	{
		string s, s2;
		ifstream file(nome);
		string possible_path = string("Scripts/") + nome + string(".lua");
		ifstream file2(possible_path);
		const char *cc;

		if (Existe(possible_path) && file2.is_open())
		{
			while (getline(file2, s))
			{
				s2 += s + "\n";
			}
		}
		else if (Existe(nome) && file.is_open())
		{
			while (getline(file, s))
			{
				s2 += s + "\n";
			}
		}
		else
		{
			s2 = "erro";
		}

		return s2;
	}

	unsigned char font_quality = 40;

	mapeamento_assets<fonte> mapeamento_fontes;
	std::mutex mapeamento_fontes_mtx;

	shared_ptr<fonte> carregar_fonte(string lugar)
	{

		std::lock_guard<std::mutex> lock(mapeamento_fontes_mtx);

		ifstream file(lugar);

		if (file)
		{
			if (mapeamento_fontes.pegar(lugar).get() == NULL && has_loading_request(lugar) == false)
			{

				add_loading_request(lugar);

				fonte f;
				f.path = lugar;
				f.quality = font_quality;

				FT_Library ft;
				if (FT_Init_FreeType(&ft))
				{
					std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
				}

				FT_Face face;

				if (FT_New_Face(ft, lugar.c_str(), 0, &face))
				{
					std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
				}

				FT_Set_Pixel_Sizes(face, 0, font_quality);

				std::map<wchar_t, caractere_info> chars;

				FT_UInt glyphIndex;
				FT_ULong charCode = FT_Get_First_Char(face, &glyphIndex);
				while (glyphIndex != 0)
				{
					FT_Load_Char(face, charCode, FT_LOAD_RENDER);
					pair<wchar_t, caractere_info> cp;

					wchar_t charcter = (wchar_t)charCode;
					cp.first = charcter;
					cp.second.char_ = charcter;
					cp.second.width = face->glyph->bitmap.width;
					cp.second.height = face->glyph->bitmap.rows;
					cp.second.left = face->glyph->bitmap_left;
					cp.second.top = face->glyph->bitmap_top;
					cp.second.pitch = face->glyph->bitmap.pitch;
					cp.second.adivancement = (float)face->glyph->advance.x / 64;

					std::vector<uint8_t> bm(cp.second.width * cp.second.height);
					for (size_t i = 0; i < cp.second.width * cp.second.height; i++)
					{
						bm[i] = face->glyph->bitmap.buffer[i];
					}
					cp.second.bitmap = bm;

					chars.insert(cp);

					charCode = FT_Get_Next_Char(face, charCode, &glyphIndex);
				}

				f.chars = chars;

				FT_Done_Face(face);
				FT_Done_FreeType(ft);

				remove_loading_request(lugar);
				return mapeamento_fontes.aplicar(lugar, f);
			}
			else
			{
				while (has_loading_request(lugar))
				{
				}
				return mapeamento_fontes.pegar(lugar);
			}
		}
		else
		{
			print("nao foi possivel carregar fonte em", lugar);
			shared_ptr<fonte> ret;
			return ret;
		}
	}

	void carregar_fonte_thread(string local, shared_ptr<fonte> *ret)
	{
		*ret = carregar_fonte(local);
	}

	void carregar_fontes_thread(string local, vector<shared_ptr<fonte> *> ret)
	{
		for (shared_ptr<fonte> *r : ret)
		{
			carregar_fonte_thread(local, r);
		}
	}

	ivec2 svg_res(255, 255);
	// 512

	bool use_texture_max_size = true;
	ivec2 texture_max_size(255, 255);

	mapeamento_assets<imagem> mapeamento_imagems;
	std::mutex mapeamento_imagems_mtx;

	shared_ptr<imagem> carregar_Imagem(string local)
	{

		std::lock_guard<std::mutex> lock(mapeamento_imagems_mtx);

		bool jaCarregada = false;

		int X = 1;
		int Y = 1;
		int canais = 4;

		unsigned char *data = NULL;

		if (Existe(local) && mapeamento_imagems.pegar(local).get() == NULL && has_loading_request(local) == false)
		{
			add_loading_request(local);
			if (obterExtensaoArquivo(local) == "svg")
			{

				X = svg_res.x;
				Y = svg_res.y;

				NSVGimage *imagemSVG = nsvgParseFromFile(local.c_str(), "px", 96.0f);

				if (!imagemSVG)
				{
					print("Error loading SVG ", local);
				}

				NSVGrasterizer *rast = nsvgCreateRasterizer();

				data = new unsigned char[X * Y * canais];

				nsvgRasterize(rast, imagemSVG, 0, 0, static_cast<float>(X) / static_cast<float>(imagemSVG->width), data, X, Y, X * canais);

				nsvgDeleteRasterizer(rast);
				nsvgDelete(imagemSVG);

				imagem image = imagem(X, Y, canais, data);
				image.local = local;

				delete[] data;

				remove_loading_request(local);
				return mapeamento_imagems.aplicar(local, image);
			}

			data = stbi_load(local.c_str(), &X, &Y, &canais, canais);

			if (use_texture_max_size && X > texture_max_size.x || Y > texture_max_size.y)
			{
				unsigned int sizex = std::min(X, texture_max_size.x);
				unsigned int sizey = std::min(Y, texture_max_size.y);
				unsigned char *data2 = new unsigned char[texture_max_size.x * texture_max_size.y * canais];
				stbir_resize_uint8(data, X, Y, 0, data2, sizex, sizey, 0, canais);

				imagem image = imagem(sizex, sizey, canais, data2);
				image.local = local;

				stbi_image_free(data);
				// delete[] data;
				delete[] data2;

				remove_loading_request(local);
				return mapeamento_imagems.aplicar(local, image);
			}
			else
			{
				imagem image = imagem(X, Y, canais, data);
				image.local = local;

				stbi_image_free(data);
				// delete[] data;

				remove_loading_request(local);
				return mapeamento_imagems.aplicar(local, image);
			}
		}
		else
		{
			while (has_loading_request(local))
			{
			}
			return mapeamento_imagems.pegar(local);
		}
	}

	shared_ptr<imagem> registrar_Imagem(string name, unsigned int res_x, unsigned int res_y, unsigned int chanals, unsigned char *data)
	{

		std::lock_guard<std::mutex> lock(mapeamento_imagems_mtx);
		add_loading_request(name);

		imagem image;

		image.local = name;
		if (use_texture_max_size && res_x > texture_max_size.x || res_x > texture_max_size.y)
		{
			unsigned int sizex = std::min((int)res_x, texture_max_size.x);
			unsigned int sizey = std::min((int)res_y, texture_max_size.y);
			unsigned char *data2 = new unsigned char[texture_max_size.x * texture_max_size.y * chanals];
			stbir_resize_uint8(data, res_x, res_y, 0, data2, sizex, sizey, 0, chanals);
			image = imagem(sizex, sizey, chanals, data2);
			delete[] data2;
		}
		else
		{
			image = imagem(res_x, res_y, chanals, data);
		}

		mapeamento_imagems.aplicar(name, image);
		remove_loading_request(name);
		return mapeamento_imagems.aplicar(name, image);
	}

	void carregar_Imagem_thread(string local, shared_ptr<imagem> *ret)
	{
		*ret = carregar_Imagem(local);
	}

	void carregar_Imagems_thread(string local, vector<shared_ptr<imagem> *> ret)
	{
		for (shared_ptr<imagem> *r : ret)
			carregar_Imagem_thread(local, r);
	}

	void salvar_imagem(unsigned char *data, ivec2 res, int canais, string local)
	{
		stbi_write_png((local + ".png").c_str(), res.x, res.y, canais, (const void *)data, res.x * canais * sizeof(char));
	}

	void salvar_imagem(shared_ptr<imagem> img, string local)
	{
		salvar_imagem(&img->data[0], ivec2(img->res.x, img->res.y), img->canais, local);
	}

	void imagem_teste()
	{

		uint8_t *data = new unsigned char[4 * 2 * 2];
		int i = 0;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255;

		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255;

		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;

		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;
		stbi_write_png("teste.png", 2, 2, 4, data, 2 * 4);
	}

	mapeamento_assets<tile_set> mapeamento_tilesets;
	std::mutex mapeamento_tilesets_mtx;
	shared_ptr<tile_set> carregar_tile_set(string local)
	{

		std::lock_guard<std::mutex> lock(mapeamento_tilesets_mtx);

		string pasta_imagems = pegar_pasta_arquivo(local);

		if (Existe(local.c_str()))
		{

			if (mapeamento_tilesets.pegar(local).get() == NULL && has_loading_request(local) == false)
			{
				add_loading_request(local);
				// pegar info tile_set
				tile_set ret = tile_set();
				ret.local = local;
				ifstream arquivo(local);
				json j;
				arquivo >> j;

				if (j.find("image") != j.end())
				{
					ret.local_imagem = pasta_imagems + j["image"].get<string>();
					// ret.local_imagem = j["image"].get<string>();
					ret.tiles_img = carregar_Imagem(ret.local_imagem);
				}

				if (j.find("tilewidth") != j.end() && j.find("tileheight") != j.end())
				{
					ret.res_tile = ivec2(j["tilewidth"].get<int>(), j["tileheight"].get<int>());
					if (ret.tiles_img != NULL)
					{
						ret.quant_tiles = ivec2(ret.tiles_img->res.x / ret.res_tile.x, ret.tiles_img->res.y / ret.res_tile.y);
					}
				}

				// pegar info tiles
				if (j.find("tiles") != j.end())
				{
					vector<json> tiles_infos = j["tiles"].get<vector<json>>();
					ret.tiles.resize(tiles_infos.size() + 1);

					ret.tiles[0] = tile();

					for (int a = 0; a < tiles_infos.size(); a++)
					{

						tile t;
						// t.sprite_id = a;
						t.sprite_id = tiles_infos[a]["id"];

						// pegar propriedade tile
						vector<json> propriedades_json = tiles_infos[a]["properties"].get<vector<json>>();
						for (int b = 0; b < propriedades_json.size(); b++)
						{
							string name = propriedades_json[b]["name"].get<string>();

							if (name.compare("name") == 0)
							{
								t.nome = propriedades_json[b]["value"].get<string>();
							}
							// else if (name.compare("collision_type") == 0) {
							//	t.tipo_colisao = propriedades_json[b]["value"].get<int>();
							// }
							// else if (name.compare("visible") == 0) {
							//	t.visivel = propriedades_json[b]["value"].get<int>();
							// }
							// else if (name.compare("interactive") == 0) {
							//	t.interativel = propriedades_json[b]["value"].get<int>();
							// }
						}

						ret.tiles[a + 1] = t;
					}
				}

				remove_loading_request(local);
				return mapeamento_tilesets.aplicar(local, ret);
			}
			else
			{
				while (has_loading_request(local))
				{
				}
				return mapeamento_tilesets.pegar(local);
			}
		}
		else
		{
			print("nao foi possivel carregar tile_set em", local);
			shared_ptr<tile_set> ret;
			return ret;
		}
	}

	void carregar_tile_set_thread(string local, shared_ptr<tile_set> *ret)
	{

		*ret = carregar_tile_set(local);
	}

	void carregar_tile_sets_thread(string local, vector<shared_ptr<tile_set> *> ret)
	{

		for (shared_ptr<tile_set> *r : ret)
		{
			carregar_tile_set_thread(local, r);
		}
	}

	mapeamento_assets<tile_map_info> mapeamento_tile_map_infos;
	std::mutex mapeamento_tile_map_infos_mtx;

	shared_ptr<tile_map_info> carregar_info_tile_map(string local)
	{

		std::lock_guard<std::mutex> lock(mapeamento_tile_map_infos_mtx);

		if (Existe(local.c_str()))
		{

			if (mapeamento_tile_map_infos.pegar(local).get() == NULL && has_loading_request(local) == false)
			{
				add_loading_request(local);
				tile_map_info ret = tile_map_info();
				ret.local = local;
				ifstream arquivo(local);
				json j;
				arquivo >> j;

				ret.res = vec2(j["width"].get<int>(), j["height"].get<int>());

				// carregar camadas
				vector<json> camadas = j["layers"].get<vector<json>>();
				ret.info.resize(camadas.size());

				for (int i = 0; i < camadas.size(); i++)
				{
					if (camadas[i]["type"].get<string>().compare("tilelayer") == 0)
					{
						ret.info[i] = camadas[i]["data"].get<vector<int>>();
					}
					else
					{
						ret.info[i].resize(ret.res.x * ret.res.y);
						for (int a = 0; a < ret.res.x * ret.res.y; a++)
						{
							ret.info[i][a] = 0;
						}
					}
				}

				remove_loading_request(local);
				return mapeamento_tile_map_infos.aplicar(local, ret);
			}
			else
			{
				while (has_loading_request(local))
				{
				}
				return mapeamento_tile_map_infos.pegar(local);
			}
		}
		else
		{
			shared_ptr<tile_map_info> ret;
			return ret;
		}
	}

	void carregar_info_tile_map_thread(string local, shared_ptr<tile_map_info> *ret)
	{

		*ret = carregar_info_tile_map(local);
	}

	void carregar_info_tile_maps_thread(string local, vector<shared_ptr<tile_map_info> *> ret)
	{

		for (shared_ptr<tile_map_info> *r : ret)
		{
			carregar_info_tile_map_thread(local, r);
		}
	}

	mapeamento_assets<cena_3D> cenas_3D;
	std::mutex cenas_3D_mtx;

	vec3 decode_obj_f(string s)
	{
		vec3 ret;
		string valores[3];
		int i = 0;
		for (char c : s)
		{
			if (c != '/')
			{
				valores[i].push_back(c);
			}
			else
			{
				i++;
			}
		}
		ret = vec3(stof(valores[0]), stof(valores[1]), stof(valores[2]));
		return ret;
	}

	struct Obj_mesh_index_struct
	{

		vector<unsigned int> v_index, vt_index, vn_index;
	};
	typedef struct Obj_mesh_index_struct Obj_mesh_index;

	shared_ptr<cena_3D> importar_obj(string local)
	{
		// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

		std::lock_guard<std::mutex> lock(cenas_3D_mtx);

		string pasta = pegar_pasta_arquivo(local);
		cena_3D ret;
		ret.nome = local;
		escrever(local);

		Material mat;

		ifstream arquivo_obj(local);
		string linha;

		string instrucao_selecionada;
		map<string, Obj_mesh_index> instrucoes;
		vector<vec3> v;
		vector<vec2> vt;
		vector<vec3> vn;

		bool s;
		string usemtl;
		vector<unsigned int> f;

		if (cenas_3D.pegar(local).get() == NULL)
		{
			while (getline(arquivo_obj, linha))
			{

				stringstream ss;
				ss << linha;

				vector<string> valor_linha;
				valor_linha.resize(4);
				ss >> valor_linha[0] >> valor_linha[1] >> valor_linha[2] >> valor_linha[3];
				if (!valor_linha[0].compare("o"))
				{
					Obj_mesh_index om;
					instrucoes.insert(pair<string, Obj_mesh_index>(valor_linha[1], om));
					instrucao_selecionada = valor_linha[1];
				}
				else if (!valor_linha[0].compare("v"))
				{
					vec3 v3;
					v3.x = stof(valor_linha[1]);
					v3.y = stof(valor_linha[2]);
					v3.z = stof(valor_linha[3]);
					v.push_back(v3);
				}
				else if (!valor_linha[0].compare("vt"))
				{
					vec2 v2;
					v2.x = stof(valor_linha[1]);
					v2.y = stof(valor_linha[2]);
					vt.push_back(v2);
				}
				else if (!valor_linha[0].compare("vn"))
				{
					vec3 v3;
					v3.x = stof(valor_linha[1]);
					v3.y = stof(valor_linha[2]);
					v3.z = stof(valor_linha[3]);
					vn.push_back(v3);
				}
				else if (!valor_linha[0].compare("usemtl"))
				{
					usemtl = valor_linha[1];
				}
				else if (!valor_linha[0].compare("s"))
				{
					map<string, bool> m = {pair<string, bool>("on", true), pair<string, bool>("off", false)};
					s = m[valor_linha[1]];
				}
				else if (!valor_linha[0].compare("f"))
				{

					vec3 a = decode_obj_f(valor_linha[1]);
					vec3 b = decode_obj_f(valor_linha[2]);
					vec3 c = decode_obj_f(valor_linha[3]);

					instrucoes[instrucao_selecionada].v_index.push_back(a.x - 1);
					instrucoes[instrucao_selecionada].v_index.push_back(b.x - 1);
					instrucoes[instrucao_selecionada].v_index.push_back(c.x - 1);

					instrucoes[instrucao_selecionada].vt_index.push_back(a.y - 1);
					instrucoes[instrucao_selecionada].vt_index.push_back(b.y - 1);
					instrucoes[instrucao_selecionada].vt_index.push_back(c.y - 1);

					instrucoes[instrucao_selecionada].vn_index.push_back(a.z - 1);
					instrucoes[instrucao_selecionada].vn_index.push_back(b.z - 1);
					instrucoes[instrucao_selecionada].vn_index.push_back(c.z - 1);
				}
			}

			for (pair<string, Obj_mesh_index> p : instrucoes)
			{

				pair<string, shared_ptr<malha>> m = pair<string, shared_ptr<malha>>(p.first, make_shared<malha>());
				// transformar em mesh

				m.second->vertices.resize(p.second.v_index.size());
				m.second->indice.resize(p.second.v_index.size());

				m.second->nome = p.first;

				for (int i = 0; i < p.second.v_index.size(); i++)
				{

					m.second->indice[i] = i;

					m.second->vertices[i].posicao[0] = v[p.second.v_index[i]].x;
					m.second->vertices[i].posicao[1] = v[p.second.v_index[i]].y;
					m.second->vertices[i].posicao[2] = v[p.second.v_index[i]].z;

					m.second->vertices[i].uv[0] = vt[p.second.vt_index[i]].x;
					m.second->vertices[i].uv[1] = vt[p.second.vt_index[i]].y;

					m.second->vertices[i].normal[0] = vn[p.second.vn_index[i]].x;
					m.second->vertices[i].normal[1] = vn[p.second.vn_index[i]].y;
					m.second->vertices[i].normal[2] = vn[p.second.vn_index[i]].z;
				}

				// adicionar malha
				m.second->arquivo_origem = local;
				m.second->comprimir();
				m.second->pegar_tamanho_maximo();

				calculate_mesh_tangent(*(m.second.get()));

				ret.malhas.insert(m);
				ret.objetos.minhas_malhas.push_back(m.second);
			}

			// adicionar material
			ret.materiais.insert(pair<string, Material>("material", mat));
			ret.objetos.meus_materiais.push_back(mat);

			return cenas_3D.aplicar(local, ret);
		}
		else
		{
			while (has_loading_request(local))
			{
			}
			return cenas_3D.pegar(local);
		}
	}

	void importar_obj_thread(string local, string local_mtl, shared_ptr<cena_3D> *ret)
	{

		*ret = importar_obj(local);
	}

	void importar_objs_thread(string local, string local_mtl, vector<shared_ptr<cena_3D> *> ret)
	{
		for (shared_ptr<cena_3D> *r : ret)
		{
			importar_obj_thread(local, local_mtl, r);
		}
	}

	json table_json(Table table)
	{
		json ret;
		for (auto &[key, value] : table.m_floatMap)
		{
			ret[key] = value;
		}
		for (auto &[key, value] : table.m_stringMap)
		{
			ret[key] = value;
		}
		for (auto &[key, value] : table.m_tableMap)
		{
			ret[key] = table_json(value);
		}
		return ret;
	}

	Table json_table(json table)
	{
		Table ret;
		for (auto it = table.begin(); it != table.end(); it++)
		{
			string key = it.key();
			nlohmann::json::value_t type = it->type();

			if (type == json::value_t::object)
			{
				ret.setTable(key, json_table(it->get<json>()));
			}
			else if (type == json::value_t::array)
			{
				ret.setTable(key, json_table(it->get<json>()));
			}
			else if (type == json::value_t::string)
			{
				ret.setString(key, it->get<string>());
			}
			else if (type == json::value_t::boolean)
			{
				ret.setFloat(key, it->get<bool>());
			}
			else if (type == json::value_t::number_integer)
			{
				ret.setFloat(key, it->get<int>());
			}
			else if (type == json::value_t::number_float)
			{
				ret.setFloat(key, it->get<float>());
			}
		}

		return ret;
	}

	std::string table_bin(const Table &table)
	{
		std::stringstream ss;
		boost::archive::text_oarchive oa(ss);
		oa << table;
		return ss.str();
	}

	Table bin_table(const std::string &table_bin)
	{
		std::stringstream ss(table_bin);
		boost::archive::text_iarchive ia(ss);
		Table table;
		ia >> table;
		return table;
	}

	void storeTableData(std::string filename, Table table)
	{
		json data = table_json(table);

		std::ofstream file(filename, std::ios::out | std::ios::binary);
		if (file.good())
		{
			std::vector<uint8_t> bson_data = json::to_bson(data);
			file.write((const char *)&bson_data[0], bson_data.size());
			file.close();
			print("Data saved to ", filename);
		}
		else
		{
			print("Error: could not save data to ", filename);
		}
	}

	Table readTableData(const std::string &filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);

		// Find the file size
		file.seekg(0, std::ios::end);
		std::streampos size = file.tellg();
		file.seekg(0, std::ios::beg);

		// Read the file into a vector
		std::vector<uint8_t> bson_data(size);
		file.read((char *)&bson_data[0], size);

		// Close the file
		file.close();

		nlohmann::json json_data = nlohmann::json::from_bson(bson_data);
		return json_table(json_data);
	}

	struct Vec3Comparator
	{
		bool operator()(const glm::vec3 &a, const glm::vec3 &b) const
		{
			return (a.x < b.x) || (a.x == b.x && (a.y < b.y || (a.y == b.y && a.z < b.z)));
		}
	};

	shared_ptr<cena_3D> importar_map(string local)
	{
		// https://github.com/stefanha/map-files/blob/master/MAPFiles.pdf
		// try this on gpt: make a c++ function that read an .map file (like the ones used on quake)

		std::lock_guard<std::mutex> lock(cenas_3D_mtx);
		cena_3D ret;
		ret.nome = local;

		Full_Map_Info map_info = read_map_file(local);

		return cenas_3D.aplicar(local, ret);
	}
	void importar_map_thread(string local, shared_ptr<cena_3D> *ret)
	{

		*ret = importar_map(local);
	}

	void importar_maps_thread(string local, vector<shared_ptr<cena_3D> *> ret)
	{

		for (shared_ptr<cena_3D> *r : ret)
		{
			importar_map_thread(local, r);
		}
	}

	std::vector<malha> converter_malha_gltf(gltf_loader::GLTFLoader gltf_loader, gltf_loader::Mesh m, string file_path)
	{

		std::vector<malha> ret;
		bool add_skin_count = false;

		size_t bone_advancement = 0;

		for (size_t a = 0; a < m.sub_meshes.size(); a++)
		{

			malha ma;
			if (a > 0)
			{
				ma.nome = m.name + "." + to_string(a);
			}
			else
			{
				ma.nome = m.name;
			}

			ma.indice = m.sub_meshes[a].indices;
			ma.arquivo_origem = file_path;

			for (int i = 0; i < m.sub_meshes[a].positions.size(); i++)
			{

				vertice v;

				v.posicao[0] = m.sub_meshes[a].positions[i].x;
				v.posicao[1] = m.sub_meshes[a].positions[i].y;
				v.posicao[2] = m.sub_meshes[a].positions[i].z;

				if (m.sub_meshes[a].normals.size() > 0)
				{
					v.normal[0] = m.sub_meshes[a].normals[i].x;
					v.normal[1] = m.sub_meshes[a].normals[i].y;
					v.normal[2] = m.sub_meshes[a].normals[i].z;
				}

				if (m.sub_meshes[a].colors.size() > 0)
				{
					v.cor[0] = m.sub_meshes[a].colors[i].x;
					v.cor[1] = m.sub_meshes[a].colors[i].y;
					v.cor[2] = m.sub_meshes[a].colors[i].z;
				}

				if (m.sub_meshes[a].texcoords.size() > 0)
				{
					v.uv[0] = m.sub_meshes[a].texcoords[i].x;
					v.uv[1] = m.sub_meshes[a].texcoords[i].y;
				}

				if (gltf_loader.skins.size() > 0)
				{
					for (int b = 0; b < MAX_BONE_INFLUENCE; ++b)
					{
						v.id_ossos[b] = 0;
						v.peso_ossos[b] = 0.0f;
						if (b < m.sub_meshes[a].BoneIDs.size())
						{
							if (!m.sub_meshes[a].BoneIDs[b].empty())
							{
								size_t BoneID = m.sub_meshes[a].BoneIDs[i][b];
								gltf_loader::Skin skin = gltf_loader.skins[m.skin];
								v.id_ossos[b] = skin.jointIndices[BoneID];
							}
						}
						if (b < m.sub_meshes[a].Weights.size())
						{
							if (!m.sub_meshes[a].Weights[b].empty())
							{
								v.peso_ossos[b] = m.sub_meshes[a].Weights[i][b];

								ma.pele = true;
							}
						}
					}
				}

				if (m.sub_meshes[a].skin)
				{
					ma.pele = true;
				}

				ma.vertices.push_back(v);
			}

			ma.comprimir();

			ret.push_back(ma);
		}

		return ret;
	}

	mat4 buscar_offset_matrices(gltf_loader::GLTFLoader loader, size_t node_id)
	{
		for (gltf_loader::Skin s : loader.skins)
		{
			for (size_t i = 0; i < s.jointIndices.size(); i++)
			{
				if (s.jointIndices[i] == node_id)
				{
					return s.inverseBindMatrices[i];
				}
			}
		}
		return mat4(1.0f);
	}

	objeto_3D node_3D_object(gltf_loader::Node node, vector<gltf_loader::Node> full_node_list, cena_3D &cena, gltf_loader::GLTFLoader loader)
	{
		objeto_3D ret;
		ret.nome = node.name;
		ret.id = node.id;

		mat4 offset_matrix = buscar_offset_matrices(loader, node.id);
		if (offset_matrix != mat4(1.0f))
		{
			cena.offset_matrices[node.id] = offset_matrix;
		}

		ret.posicao = node.translation;
		ret.quaternion = node.rotation;
		ret.escala = node.scale;

		for (int i = 0; i < node.meshIndices.size(); i++)
		{

			int mesh_index = node.meshIndices[i];
			string mesh_name = loader.meshes[mesh_index].name;
			ret.minhas_malhas.push_back(cena.malhas[mesh_name]);

			int material_index = loader.meshes[mesh_index].sub_meshes[0].material;

			if (material_index < loader.materials.size() && loader.materials.size() != 0)
			{
				string material_name = loader.materials[material_index].name;
				ret.meus_materiais.push_back(cena.materiais[material_name]);
			}
			else
			{

				Material mat;
				mat.shader = "mesh";
				mat.texturas[0] = carregar_Imagem("Textures/white.png");
				ret.meus_materiais.push_back(mat);
			}

			int b = 1;
			while (true)
			{

				string sub_mesh_name = mesh_name + "." + to_string(b);
				if (cena.malhas.find(sub_mesh_name) != cena.malhas.end())
				{
					ret.minhas_malhas.push_back(cena.malhas[sub_mesh_name]);

					if (b < loader.meshes[mesh_index].sub_meshes.size())
					{
						int material_index = loader.meshes[mesh_index].sub_meshes[b].material;
						if (material_index <= loader.materials.size() - 1 && loader.materials.size() != 0)
						{
							string material_name = loader.materials[material_index].name;
							ret.meus_materiais.push_back(cena.materiais[material_name]);
						}
					}
					else
					{
						Material mat;
						mat.shader = "mesh";
						mat.texturas[0] = carregar_Imagem("Textures/null.png");
						ret.meus_materiais.push_back(mat);
					}

					b++;
				}
				else
				{
					break;
				}
			}
		}

		// correct position in objects with no nodes
		if (node.meshIndices.size() == 0)
		{
			// ret.posicao.x = -node.translation.x;
			// ret.posicao.z = -node.translation.z;
		}

		ret.variaveis = json_table(node.extras);

		for (int i = 0; i < node.childrenIndices.size(); i++)
		{
			ret.filhos.push_back(node_3D_object(full_node_list[node.childrenIndices[i]], full_node_list, cena, loader));
		}

		return ret;
	}

	shared_ptr<cena_3D> importar_gltf(string local)
	{

		// skin_count = 0;

		cena_3D ret;

		if (cenas_3D.pegar(local).get() == NULL && has_loading_request(local) == false)
		{

			add_loading_request(local);
			gltf_loader::GLTFLoader gltf_loader(local);
			gltf_loader.load();

			for (int i = 0; i < gltf_loader.meshes.size(); i++)
			{
				vector<malha> malhas = converter_malha_gltf(gltf_loader, gltf_loader.meshes[i], local);
				for (malha m : malhas)
				{
					ret.malhas.insert(pair<string, shared_ptr<malha>>(m.nome, make_shared<malha>(m)));
				}
			}

			for (int i = 0; i < gltf_loader.textures.size(); i++)
			{
				string svg_uri = trocarExtensaoArquivo(gltf_loader.textures[i].uri, "svg");
				if (ManuseioDados::Existe(svg_uri))
				{
					ret.texturas[gltf_loader.textures[i].uri] = carregar_Imagem(svg_uri);
				}
				else if (ManuseioDados::Existe(gltf_loader.textures[i].uri))
				{
					ret.texturas[gltf_loader.textures[i].uri] = carregar_Imagem(gltf_loader.textures[i].uri);
				}
				else
				{
					ret.texturas[gltf_loader.textures[i].uri] = ManuseioDados::carregar_Imagem("Textures/white.png");
				}
			}

			for (int i = 0; i < gltf_loader.materials.size(); i++)
			{
				Material mat;
				mat.shader = "mesh";
				mat.cor = gltf_loader.materials[i].baseColorFactor;

				if (gltf_loader.materials[i].textureIndex > -1)
				{
					string image_location = gltf_loader.textures[gltf_loader.materials[i].textureIndex].uri;
					mat.texturas[0] = ret.texturas[image_location];

					if (mat.cor.x == 0 and mat.cor.y == 0 and mat.cor.z == 0 and mat.cor.w == 0)
					{
						mat.cor.x = 1;
						mat.cor.y = 1;
						mat.cor.z = 1;
						mat.cor.w = 1;
					}
				}
				else
				{
					mat.texturas[0] = ManuseioDados::carregar_Imagem("Textures/white.png");
				}

				ret.materiais[gltf_loader.materials[i].name] = mat;
			}

			for (int a = 0; a < gltf_loader.scenes.size(); a++)
			{
				for (int b = 0; b < gltf_loader.scenes[a].nodesIndices.size(); b++)
				{
					int node_index = gltf_loader.scenes[a].nodesIndices[b];
					gltf_loader::Node n = gltf_loader.nodes[node_index];
					objeto_3D ob_3D = node_3D_object(n, gltf_loader.nodes, ret, gltf_loader);
					ret.objetos.filhos.push_back(ob_3D);
				}
			}

			for (gltf_loader::Animation a : gltf_loader.animations)
			{
				animacao ani;

				ani.nome = a.name;
				ani.duration = a.duration;
				ani.start_time = a.start_time;

				ani.keyFrames = {};
				for (vector<gltf_loader::AnimationKeyFrame> kfs : a.keyFrames)
				{
					vector<key_frame> okfs;
					for (gltf_loader::AnimationKeyFrame kf : kfs)
					{
						key_frame okf;

						okf.object_id = kf.targetNodeIndex;

						okf.has_position = kf.has_position;
						okf.has_rotation = kf.has_rotation;
						okf.has_scale = kf.has_scale;

						okf.position = kf.position;

						okf.rotation = kf.rotation;
						okf.scale = kf.scale;

						okfs.push_back(okf);
					}
					ani.keyFrames.push_back(okfs);
				}

				ret.animacoes.insert(pair<string, animacao>(a.name, ani));
			}

			ret.nome = local;

			remove_loading_request(local);
			return cenas_3D.aplicar(local, ret);
		}
		while (has_loading_request(local))
		{
		}
		return cenas_3D.pegar(local);
	}

	// tiny

	bool TinyGLTFLoadImageData(tinygltf::Image *image, const int image_idx, std::string *err, std::string *warn, int req_width, int req_height, const unsigned char *bytes, int size, void *user_data)
	{
		(void)warn;

		tinygltf::LoadImageDataOption option;
		if (user_data)
		{
			option = *reinterpret_cast<tinygltf::LoadImageDataOption *>(user_data);
		}

		int w = 0, h = 0, comp = 0, req_comp = 0;

		unsigned char *data = nullptr;

		// preserve_channels true: Use channels stored in the image file.
		// false: force 32-bit textures for common Vulkan compatibility. It appears
		// that some GPU drivers do not support 24-bit images for Vulkan
		req_comp = option.preserve_channels ? 0 : 4;
		int bits = 8;
		int pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;

		// It is possible that the image we want to load is a 16bit per channel image
		// We are going to attempt to load it as 16bit per channel, and if it worked,
		// set the image data accordingly. We are casting the returned pointer into
		// unsigned char, because we are representing "bytes". But we are updating
		// the Image metadata to signal that this image uses 2 bytes (16bits) per
		// channel:
		if (stbi_is_16_bit_from_memory(bytes, size))
		{
			data = reinterpret_cast<unsigned char *>(
				stbi_load_16_from_memory(bytes, size, &w, &h, &comp, req_comp));
			if (data)
			{
				bits = 16;
				pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
			}
		}

		// at this point, if data is still NULL, it means that the image wasn't
		// 16bit per channel, we are going to load it as a normal 8bit per channel
		// image as we used to do:
		// if image cannot be decoded, ignore parsing and keep it by its path
		// don't break in this case
		// FIXME we should only enter this function if the image is embedded. If
		// image->uri references
		// an image file, it should be left as it is. Image loading should not be
		// mandatory (to support other formats)
		if (!data)
			data = stbi_load_from_memory(bytes, size, &w, &h, &comp, req_comp);
		if (!data)
		{
			// NOTE: you can use `warn` instead of `err`
			if (err)
			{
				(*err) +=
					"Unknown image format. STB cannot decode image data for image[" +
					std::to_string(image_idx) + "] name = \"" + image->name + "\".\n";
			}
			return false;
		}

		if ((w < 1) || (h < 1))
		{
			stbi_image_free(data);
			if (err)
			{
				(*err) += "Invalid image data for image[" + std::to_string(image_idx) +
						  "] name = \"" + image->name + "\"\n";
			}
			return false;
		}

		if (req_width > 0)
		{
			if (req_width != w)
			{
				stbi_image_free(data);
				if (err)
				{
					(*err) += "Image width mismatch for image[" +
							  std::to_string(image_idx) + "] name = \"" + image->name +
							  "\"\n";
				}
				return false;
			}
		}

		if (req_height > 0)
		{
			if (req_height != h)
			{
				stbi_image_free(data);
				if (err)
				{
					(*err) += "Image height mismatch. for image[" +
							  std::to_string(image_idx) + "] name = \"" + image->name +
							  "\"\n";
				}
				return false;
			}
		}

		if (req_comp != 0)
		{
			// loaded data has `req_comp` channels(components)
			comp = req_comp;
		}

		image->width = w;
		image->height = h;
		image->component = comp;
		image->bits = bits;
		image->pixel_type = pixel_type;
		image->image.resize(static_cast<size_t>(w * h * comp) * size_t(bits / 8));
		std::copy(data, data + w * h * comp * (bits / 8), image->image.begin());
		stbi_image_free(data);

		return true;
	}

	pair<string, Material> tgl_material_convert(string local, tinygltf::Model model, tinygltf::Material in_mat)
	{

		pair<string, Material> ret;
		ret.first = in_mat.name;
		ret.second.shader = "mesh";

		if (in_mat.pbrMetallicRoughness.baseColorTexture.index > -1)
		{

			const int texture_index = in_mat.pbrMetallicRoughness.baseColorTexture.index;
			const tinygltf::Texture &texture = model.textures[texture_index];
			const tinygltf::Image &image = model.images[texture.source];
			string name = image.name;
			ret.second.texturas[0] = carregar_Imagem(name);

			// print("texturas",name,ret.second.texturas[0]->local,ret.second.texturas[0]->res.x,ret.second.texturas[0]->res.y);
		}
		else
		{
			ret.second.texturas[0] = carregar_Imagem("Textures/white.png");
		}

		std::vector<double> bcf = in_mat.pbrMetallicRoughness.baseColorFactor;
		ret.second.cor = vec4(bcf[0], bcf[1], bcf[2], bcf[3]);
		if(ret.second.cor.w == 1 && in_mat.alphaMode != "OPAQUE"){
			
			ret.second.cor.w == 0.99;
		}

		ret.second.suave = in_mat.pbrMetallicRoughness.roughnessFactor;
		ret.second.metalico = in_mat.pbrMetallicRoughness.metallicFactor;
		ret.second.uv_pos_sca = vec4(0, 0, 1, 1);

		return ret;
	}

	void tgl_convertAccessorData(const tinygltf::Model &model, const tinygltf::Accessor &accessor, std::vector<float> &data)
	{
		const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
		const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

		size_t count = accessor.count;
		size_t byteStride = accessor.ByteStride(bufferView);

		const uint8_t *bufferDataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

		// Determine the number of elements per type
		size_t numElements = 1;
		switch (accessor.type)
		{
		case TINYGLTF_TYPE_SCALAR:
			numElements = 1;
			break;
		case TINYGLTF_TYPE_VEC2:
			numElements = 2;
			break;
		case TINYGLTF_TYPE_VEC3:
			numElements = 3;
			break;
		case TINYGLTF_TYPE_VEC4:
			numElements = 4;
			break;
		case TINYGLTF_TYPE_MAT4:
			numElements = 16;
			break;
		default:
			std::cerr << "Unsupported accessor type!" << std::endl;
			return;
		}

		size_t componentSize = 0;
		switch (accessor.componentType)
		{
		case TINYGLTF_COMPONENT_TYPE_BYTE:
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			componentSize = sizeof(uint8_t);
			break;
		case TINYGLTF_COMPONENT_TYPE_SHORT:
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			componentSize = sizeof(uint16_t);
			break;
		case TINYGLTF_COMPONENT_TYPE_INT:
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			componentSize = sizeof(uint32_t);
			break;
		case TINYGLTF_COMPONENT_TYPE_FLOAT:
			componentSize = sizeof(float);
			break;
		case TINYGLTF_COMPONENT_TYPE_DOUBLE:
			componentSize = sizeof(double);
			break;
		default:
			std::cerr << "Unsupported component type!" << std::endl;
			return;
		}

		for (size_t i = 0; i < count; ++i)
		{
			for (size_t j = 0; j < numElements; ++j)
			{
				// Calculate the byte offset for the current element
				size_t elementByteOffset = byteStride * i + componentSize * j;

				// Use componentType to read the data correctly
				switch (accessor.componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_BYTE:
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				{
					uint8_t value;
					memcpy(&value, bufferDataPtr + elementByteOffset, sizeof(uint8_t));
					data.push_back(static_cast<float>(value));
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_SHORT:
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				{
					uint16_t value;
					memcpy(&value, bufferDataPtr + elementByteOffset, sizeof(uint16_t));
					data.push_back(static_cast<float>(value));
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_INT:
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
				{
					uint32_t value;
					memcpy(&value, bufferDataPtr + elementByteOffset, sizeof(uint32_t));
					data.push_back(static_cast<float>(value));
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_FLOAT:
				{
					float value;
					memcpy(&value, bufferDataPtr + elementByteOffset, sizeof(float));
					data.push_back(value);
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_DOUBLE:
				{
					double value;
					memcpy(&value, bufferDataPtr + elementByteOffset, sizeof(double));
					data.push_back(static_cast<float>(value));
					break;
				}
				default:
					std::cerr << "Unsupported component type!" << std::endl;
					break;
				}
			}
		}
	}

	void tgl_convertPrimitiveToVertices(const tinygltf::Model &model, const tinygltf::Primitive &primitive, std::vector<vertice_struct> &vertices, tinygltf::Skin *skin)
	{
		vertice_struct vert;

		const tinygltf::Accessor &positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
		std::vector<float> positionData;
		tgl_convertAccessorData(model, positionAccessor, positionData);

		const tinygltf::Accessor &uvAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
		std::vector<float> uvData;
		tgl_convertAccessorData(model, uvAccessor, uvData);

		const tinygltf::Accessor &normalAccessor = model.accessors[primitive.attributes.at("NORMAL")];
		std::vector<float> normalData;
		tgl_convertAccessorData(model, normalAccessor, normalData);

		std::vector<float> colorData;
		if (primitive.attributes.find("COLOR_0") != primitive.attributes.end())
		{
			const tinygltf::Accessor &colorAccessor = model.accessors[primitive.attributes.at("COLOR_0")];
			tgl_convertAccessorData(model, colorAccessor, colorData);
		}

		std::vector<int> jointsData;
		if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end() && skin != NULL)
		{

			const tinygltf::Accessor &jointsAccessor = model.accessors[primitive.attributes.at("JOINTS_0")];
			std::vector<float> jointsDataFloat;
			tgl_convertAccessorData(model, jointsAccessor, jointsDataFloat);
			jointsData.resize(jointsDataFloat.size());
			for (size_t i = 0; i < jointsDataFloat.size(); ++i)
			{
				jointsData[i] = skin->joints[static_cast<size_t>(jointsDataFloat[i])];
			}
		}

		std::vector<float> weightsData;
		if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end())
		{
			const tinygltf::Accessor &weightsAccessor = model.accessors[primitive.attributes.at("WEIGHTS_0")];
			tgl_convertAccessorData(model, weightsAccessor, weightsData);
		}

		size_t vertexCount = positionAccessor.count;
		for (size_t i = 0; i < vertexCount; ++i)
		{
			vert.posicao[0] = positionData[i * 3];
			vert.posicao[1] = positionData[i * 3 + 1];
			vert.posicao[2] = positionData[i * 3 + 2];

			vert.uv[0] = uvData[i * 2];
			vert.uv[1] = uvData[i * 2 + 1];

			vert.normal[0] = normalData[i * 3];
			vert.normal[1] = normalData[i * 3 + 1];
			vert.normal[2] = normalData[i * 3 + 2];

			if (!colorData.empty())
			{
				vert.cor[0] = colorData[i * 3];
				vert.cor[1] = colorData[i * 3 + 1];
				vert.cor[2] = colorData[i * 3 + 2];
			}
			else
			{
				vert.cor[0] = 1;
				vert.cor[1] = 1;
				vert.cor[2] = 1;
			}

			if (!jointsData.empty() && !weightsData.empty())
			{

				
				for (int j = 0; j < MAX_BONE_INFLUENCE; ++j)
				{
					const int jw_id = (i * MAX_BONE_INFLUENCE) + j;
					vert.id_ossos[j] = (static_cast<int>(jointsData[jw_id]));
					vert.peso_ossos[j] = (static_cast<float>(weightsData[jw_id]));
				}
			}

			vertices.push_back(vert);
		}
	}

	vector<pair<string, shared_ptr<malha>>> tgl_converter_malha(string local, tinygltf::Model model, tinygltf::Skin *skin, int node_id, int mesh_id, set<string> &meshes_included)
	{
		vector<pair<string, shared_ptr<malha>>> ret = {};

		if (mesh_id > -1 && meshes_included.find(model.meshes[mesh_id].name) == meshes_included.end())
		{

			tinygltf::Node node = model.nodes[node_id];
			tinygltf::Mesh mesh = model.meshes[mesh_id];
			string base_mesh_name = mesh.name + ":";

			int i = 0;
			for (tinygltf::Primitive p : mesh.primitives)
			{
				string mesh_name = base_mesh_name + to_string(i);

				std::vector<unsigned int> indice = {};
				std::vector<vertice> vertices = {};

				tgl_convertPrimitiveToVertices(model, p, vertices, skin);

				const tinygltf::Accessor &indexAccessor = model.accessors[p.indices];
				const tinygltf::BufferView &indexBufferView = model.bufferViews[indexAccessor.bufferView];
				const tinygltf::Buffer &indexBuffer = model.buffers[indexBufferView.buffer];

				size_t indexCount = indexAccessor.count;

				if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
				{
					const uint8_t *indexDataPtr = reinterpret_cast<const uint8_t *>(indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset);

					for (size_t i = 0; i < indexCount; ++i)
					{
						if (indexDataPtr[i] < vertices.size())
						{
							// print("uint8_t indexDataPtr", (unsigned int)indexDataPtr[i]);
							indice.push_back((unsigned int)indexDataPtr[i]);
						}
					}
				}
				else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
				{
					const uint16_t *indexDataPtr = reinterpret_cast<const uint16_t *>(indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset);
					for (size_t i = 0; i < indexCount; ++i)
					{
						if (indexDataPtr[i] < vertices.size())
						{
							// print("uint16_t indexDataPtr", (unsigned int)indexDataPtr[i]);
							indice.push_back((unsigned int)indexDataPtr[i]);
						}
					}
				}
				else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
				{
					const uint32_t *indexDataPtr = reinterpret_cast<const uint32_t *>(indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset);
					for (size_t i = 0; i < indexCount; ++i)
					{
						if (indexDataPtr[i] < vertices.size())
						{
							// print("uint32_t indexDataPtr", (unsigned int)indexDataPtr[i]);
							indice.push_back((unsigned int)indexDataPtr[i]);
						}
					}
				}
				else
				{
					throw std::runtime_error("Unsupported index component type.");
				}

				malha ret_malha;
				ret_malha.nome = mesh_name;
				ret_malha.arquivo_origem = local;
				ret_malha.indice = indice;
				ret_malha.vertices = vertices;
				if (skin)
				{
					ret_malha.pele = true;
				}
				ret_malha.comprimir();

				// print("ret_malha.nome",ret_malha.nome);

				ret.push_back(pair<string, shared_ptr<malha>>(mesh_name, make_shared<malha>(ret_malha)));

				i++;
			}

			if (node.skin > -1)
			{
				tinygltf::Skin skin = model.skins[node.skin];
			}
		}

		return ret;
	}

	Table tgl_value_to_table(tinygltf::Value value)
	{
		Table table;
		if (value.IsObject())
		{
			for (const auto &key : value.Keys())
			{
				const tinygltf::Value &innerValue = value.Get(key);

				if (innerValue.IsNumber())
				{
					table.m_floatMap[key] = innerValue.GetNumberAsDouble();
				}
				else if (innerValue.IsString())
				{
					table.m_stringMap[key] = innerValue.Get<std::string>();
				}
				else if (innerValue.IsObject())
				{
					Table innerTable;
					innerTable = tgl_value_to_table(innerValue);
					table.m_tableMap[key] = innerTable;
				}
				// Você pode adicionar mais casos para outros tipos, se necessário
			}
		}
		return table;
	}

	glm::mat4 tgl_getInverseBindMatrices(const tinygltf::Model &model, const tinygltf::Skin &skin, size_t node_id)
	{
		glm::mat4 matrix = glm::mat4(1.0f); // Default to identity matrix

		// Find the joint index corresponding to the node_id
		auto jointIndexIt = std::find(skin.joints.begin(), skin.joints.end(), node_id);
		if (jointIndexIt != skin.joints.end() && skin.inverseBindMatrices >= 0)
		{
			size_t jointIndex = std::distance(skin.joints.begin(), jointIndexIt);
			const tinygltf::Accessor &accessor = model.accessors[skin.inverseBindMatrices];
			std::vector<float> matrixData;
			tgl_convertAccessorData(model, accessor, matrixData);

			// Ensure we have the right amount of data for a single matrix
			if (matrixData.size() >= (jointIndex + 1) * 16)
			{
				size_t startIndex = jointIndex * 16;
				for (size_t i = 0; i < 16; ++i)
				{
					size_t row = i / 4;
					size_t col = i % 4;
					matrix[row][col] = matrixData[startIndex + i];
				}
			}
		}

		return matrix;
	}

	objeto_3D tgl_node_convert(string local, map<string, shared_ptr<malha>> &meshes, tinygltf::Model model, int node_id, set<string> &meshes_included)
	{
		objeto_3D ret;

		tinygltf::Node node = model.nodes[node_id];

		ret.nome = node.name;

		ret.id = node_id;

		if (node.translation.size() > 0)
		{
			ret.posicao = vec3(node.translation[0], node.translation[1], node.translation[2]);
		}
		if (node.rotation.size() > 0)
		{
			ret.quaternion = quat(node.rotation[3],node.rotation[0], node.rotation[1], node.rotation[2]);
		}
		if (node.scale.size() > 0)
		{
			ret.escala = vec3(node.scale[0], node.scale[1], node.scale[2]);
		}

		if (node.mesh > -1)
		{

			tinygltf::Mesh mesh = model.meshes[node.mesh];

			if (node.skin > -1)
			{
				tinygltf::Skin skin = model.skins[node.skin];

				for (pair<string, shared_ptr<malha>> p : tgl_converter_malha(local, model, &skin, node_id, node.mesh, meshes_included))
				{
					ret.minhas_malhas.push_back(p.second);
					meshes.insert(p);
				}
			}
			else
			{
				for (pair<string, shared_ptr<malha>> p : tgl_converter_malha(local, model, NULL, node_id, node.mesh, meshes_included))
				{
					ret.minhas_malhas.push_back(p.second);
					meshes.insert(p);
				}
			}

			for (tinygltf::Primitive p : mesh.primitives)
			{
				if (p.material > -1)
				{
					ret.meus_materiais.push_back(tgl_material_convert(local, model, model.materials[p.material]).second);
				}
				else
				{
					Material mat;
					mat.shader = "mesh";
					mat.texturas[0] = carregar_Imagem("Textures/white.png");
					ret.meus_materiais.push_back(mat);
				}
			}
		}

		ret.variaveis = tgl_value_to_table(node.extras);

		for (int i : node.children)
		{
			ret.filhos.push_back(tgl_node_convert(local, meshes, model, i, meshes_included));
		}

		return ret;
	}

	vec2 tgl_getAnimationTimeDuration(const tinygltf::Model &model, const tinygltf::Animation &gltfAnimation)
	{
		float startTime = std::numeric_limits<float>::max();
		float endTime = std::numeric_limits<float>::min();

		// Iterar sobre os canais de animação
		for (const auto &channel : gltfAnimation.channels)
		{
			// Verificar se o canal tem um sampler válido
			if (channel.sampler >= 0 && channel.sampler < gltfAnimation.samplers.size())
			{
				const auto &sampler = gltfAnimation.samplers[channel.sampler];

				// Verificar se o sampler tem entrada (input) e saída (output) válidos
				if (sampler.input >= 0 && sampler.input < model.accessors.size() &&
					sampler.output >= 0 && sampler.output < model.accessors.size())
				{
					const auto &inputAccessor = model.accessors[sampler.input];
					const auto &outputAccessor = model.accessors[sampler.output];
					const auto &inputBufferView = model.bufferViews[inputAccessor.bufferView];
					const auto &inputBuffer = model.buffers[inputBufferView.buffer];

					// Interpretar os tempos de entrada
					const int count = inputAccessor.count;
					for (int i = 0; i < count; ++i)
					{
						const float time = *reinterpret_cast<const float *>(&inputBuffer.data[inputBufferView.byteOffset + i * sizeof(float)]);
						startTime = std::min(startTime, time);
						endTime = std::max(endTime, time);
					}
				}
			}
		}

		// Se não houver canais de animação válidos, definir tempo de início e duração como 0
		if (startTime == std::numeric_limits<float>::max() || endTime == std::numeric_limits<float>::min())
		{
			startTime = 0.0f;
			endTime = 0.0f;
		}

		return vec2(startTime, endTime - startTime);
	}

	std::vector<key_frame> get_keyframe(const tinygltf::Model &model, tinygltf::Animation &animation, float time)
	{
		std::vector<key_frame> ret;

		/**/

		for (const auto &channel : animation.channels)
		{
			key_frame kf;
			kf.object_id = channel.target_node; // Assume node ID maps directly to object ID

			const tinygltf::AnimationSampler &sampler = animation.samplers[channel.sampler];
			const tinygltf::Accessor &input = model.accessors[sampler.input];
			const tinygltf::Accessor &output = model.accessors[sampler.output];
			const tinygltf::BufferView &inputBufferView = model.bufferViews[input.bufferView];
			const tinygltf::BufferView &outputBufferView = model.bufferViews[output.bufferView];
			const tinygltf::Buffer &inputBuffer = model.buffers[inputBufferView.buffer];
			const tinygltf::Buffer &outputBuffer = model.buffers[outputBufferView.buffer];

			// Accessor data as raw bytes
			const unsigned char *inputData = &inputBuffer.data[inputBufferView.byteOffset + input.byteOffset];
			const unsigned char *outputData = &outputBuffer.data[outputBufferView.byteOffset + output.byteOffset];

			// Find the closest frame(s)
			for (size_t i = 0; i < input.count - 1; ++i)
			{
				float frameTime0 = *((float *)(inputData + i * sizeof(float)));
				float frameTime1 = *((float *)(inputData + (i + 1) * sizeof(float)));

				if (time >= frameTime0 && time <= frameTime1)
				{
					float t = (time - frameTime0) / (frameTime1 - frameTime0);

					// Depending on the channel.target_path, interpolate the right data
					if (channel.target_path == "translation")
					{
						glm::vec3 pos0 = *((glm::vec3 *)(outputData + i * 3 * sizeof(float)));
						glm::vec3 pos1 = *((glm::vec3 *)(outputData + (i + 1) * 3 * sizeof(float)));
						kf.position = glm::mix(pos0, pos1, t);
						kf.has_position = true;
					}
					else if (channel.target_path == "scale")
					{
						glm::vec3 scale0 = *((glm::vec3 *)(outputData + i * 3 * sizeof(float)));
						glm::vec3 scale1 = *((glm::vec3 *)(outputData + (i + 1) * 3 * sizeof(float)));
						kf.scale = glm::mix(scale0, scale1, t);
						kf.has_scale = true;
					}
					else if (channel.target_path == "rotation")
					{
						glm::quat rot0 = *((glm::quat *)(outputData + i * 4 * sizeof(float)));
						glm::quat rot1 = *((glm::quat *)(outputData + (i + 1) * 4 * sizeof(float)));
						kf.rotation = glm::slerp(rot0, rot1, t);
						kf.has_rotation = true;
					}
					break;
				}
			}

			ret.push_back(kf);
		}

		return ret;
	}

	animacao tgl_convertAnimation(tinygltf::Model model, tinygltf::Animation &gltfAnimation)
	{
		animacao result;
		result.nome = gltfAnimation.name;

		vec2 duration = tgl_getAnimationTimeDuration(model, gltfAnimation);
		result.start_time = duration.x;
		result.duration = duration.y;

		for (float t = duration.x; t < duration.x + duration.y; t += 1.0 / ANIMATION_FPS_COUNT)
		{
			t = std::min(t, duration.x + duration.y);
			result.keyFrames.push_back(get_keyframe(model, gltfAnimation, t));
		}

		return result;
	}

	pair<string, animacao> tgl_animation_convert(string local, tinygltf::Model model, tinygltf::Animation animatior)
	{
		pair<string, animacao> ret;

		ret.first = animatior.name;
		ret.second = tgl_convertAnimation(model, animatior);
		ret.second.nome = ret.first;

		return ret;
	}

	shared_ptr<cena_3D> importar_glb(string local)
	{

		if (cenas_3D.pegar(local).get() == NULL && has_loading_request(local) == false)
		{
			add_loading_request(local);

			cena_3D ret;

			tinygltf::Model model;
			tinygltf::TinyGLTF loader;

			loader.SetImageLoader(&TinyGLTFLoadImageData, nullptr);

			std::string err;
			std::string warn;
			bool work;

			if (obterExtensaoArquivo(local) == "glb")
			{
				work = loader.LoadBinaryFromFile(&model, &err, &warn, local);
			}
			else
			{
				work = loader.LoadASCIIFromFile(&model, &err, &warn, local);
			}

			if (!warn.empty())
			{
				std::cout << "Warning: " << warn << std::endl;
			}

			if (!err.empty())
			{
				std::cerr << "Erro: " << err << std::endl;
			}

			if (!work)
			{
				std::cerr << "Falha ao carregar o arquivo GLB." << std::endl;
			}

			// std::cout << "Número de texturas (images): " << model.images.size() << std::endl;
			for (tinygltf::Image i : model.images)
			{
				string name = i.name;
				if (!carregar_Imagem(name))
				{
					
					shared_ptr<imagem> img = registrar_Imagem(name, i.width, i.height, i.component, i.image.data());
					img->local = name;
					ret.texturas.insert(pair<string, shared_ptr<imagem>>(name, img));
				}
				else
				{
					shared_ptr<imagem> img = carregar_Imagem(name);
					img->local = name;
					ret.texturas.insert(pair<string, shared_ptr<imagem>>(name, img));
				}
			}

			// std::cout << "Número de materiais: " << model.materials.size() << std::endl;
			for (tinygltf::Material m : model.materials)
			{
				ret.materiais.insert(tgl_material_convert(local, model, m));
			}

			/*
			std::cout << "Número de scenes: " << model.scenes.size() << std::endl;
			std::cout << "Número de nodes: " << model.nodes.size() << std::endl;
			std::cout << "Número de skins: " << model.skins.size() << std::endl;
			std::cout << "Número de malhas (meshes): " << model.meshes.size() << std::endl;
			*/

			set<string> meshes_included;
			for (int i : model.scenes[0].nodes)
			{
				ret.objetos.filhos.push_back(tgl_node_convert(local, ret.malhas, model, i, meshes_included));
			}

			// std::cout << "Número de animações: " << model.animations.size() << std::endl;
			for (tinygltf::Animation a : model.animations)
			{
				ret.animacoes.insert(tgl_animation_convert(local, model, a));
			}

			for (tinygltf::Skin skin : model.skins)
			{
				for (int joint : skin.joints)
				{
					if (ret.offset_matrices.find(joint) == ret.offset_matrices.end())
					{
						ret.offset_matrices[joint] = tgl_getInverseBindMatrices(model, skin, joint);
					}
				}
			}

			ret.nome = local;
			remove_loading_request(local);
			return cenas_3D.aplicar(local, ret);
		}

		while (has_loading_request(local))
		{
		}
		return cenas_3D.pegar(local);
	}

	// arquivo_origem
	map<string, shared_ptr<cena_3D> (*)(string)> funcoes_abrir_modelos_3D = {
		pair<string, shared_ptr<cena_3D> (*)(string)>(".obj", importar_obj),
		pair<string, shared_ptr<cena_3D> (*)(string)>(".gltf", importar_gltf),
		// pair<string, shared_ptr<cena_3D> (*)(string)>(".gltf", importar_glb),
		pair<string, shared_ptr<cena_3D> (*)(string)>(".glb", importar_glb),
	};

	shared_ptr<cena_3D> carregar_modelo_3D(string local)
	{
		std::lock_guard<std::mutex> lock(cenas_3D_mtx);
		if (ManuseioDados::Existe(local))
		{
			shared_ptr<cena_3D> ret = funcoes_abrir_modelos_3D[pegar_estencao_arquivo(local)](local);
			return ret;
		}
		else
		{
			print("erro local ", local, " nao existe");
		}
		return NULL;
	}
	void carregar_modelo_3D_thread(string local, shared_ptr<cena_3D> *ret)
	{
		*ret = carregar_modelo_3D(local);
	}

	shared_ptr<malha> carregar_malha(string local, string nome)
	{
		return carregar_modelo_3D(local)->malhas[nome];
	}
	void carregar_malha_thread(string local, string nome, shared_ptr<malha> *ret)
	{
		*ret = carregar_malha(local, nome);
	}
	void carregar_malhas_thread(string local, string nome, vector<shared_ptr<malha> *> ret)
	{
		for (shared_ptr<malha> *r : ret)
		{
			carregar_malha_thread(local, nome, r);
		}
	}
}
