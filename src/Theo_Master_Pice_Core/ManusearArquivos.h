#pragma once
#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <thread>

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

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb-master/stb_image_write.h>



#include "read_map_file.h"

#define ANIMATION_FPS_COUNT 20
#include "read_gltf_file.h"

namespace ManuseioDados
{

	string diretorio;

	bool Existe(const char *nome)
	{
		ifstream file(nome);
		if (!file)
		{
			return false;
		}
		else
		{
			return true;
		}
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
		string possible_path = string("resources/Scripts/") + nome + string(".lua");
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

	template <typename X>
	const char *Carregar(X nome)
	{
		string s2 = Carregar_string(nome);

		return s2.c_str();
	}

	mapeamento_assets<fonte> mapeamento_fontes;

	shared_ptr<fonte> carregar_fonte(string lugar)
	{
		ifstream file(lugar);

		if (file)
		{

			if (mapeamento_fontes.pegar(lugar).get() == NULL)
			{

				string S;
				file >> S;

				fonte f;

				json JSON = json::parse(S);

				f.pixel_perfect = (bool)JSON["pixel_perfect"].get<int>();
				f.quality = JSON["quality"].get<int>();
				std::map<wchar_t, caractere_info> chars;
				vector<json> chars_json = JSON["chars"].get<vector<json>>();
				for (json c : chars_json)
				{
					pair<wchar_t, caractere_info> cp;
					wchar_t charcter = (wchar_t)c["char"].get<int>();
					cp.first = charcter;
					cp.second.char_ = charcter;
					cp.second.width = c["width"].get<int>();
					cp.second.height = c["height"].get<int>();
					cp.second.left = c["left"].get<float>();
					cp.second.top = c["top"].get<float>();
					cp.second.pitch = c["pitch"].get<float>();
					cp.second.adivancement = c["adivancement"].get<float>();

					cp.second.bitmap = c["bitmap"].get<vector<unsigned char>>();

					chars.insert(cp);
				}
				f.chars = chars;
				f.path = lugar;
				mapeamento_fontes.aplicar(lugar, f);
				return mapeamento_fontes.pegar(lugar);
			}
			else
			{
				return mapeamento_fontes.pegar(lugar);
			}
		}
		else
		{
			cout << "nao foi possivel carregar " << lugar << endl;
			shared_ptr<fonte> ret;
			return ret;
		}
	}

	void carregar_fonte_thread(string local, shared_ptr<fonte> *ret)
	{
		*ret = carregar_fonte(local);
	}

	mapeamento_assets<imagem> mapeamento_imagems;

	shared_ptr<imagem> carregar_Imagem(string local)
	{
		bool jaCarregada = false;

		int X = 1, Y = 1, canais = 4;

		unsigned char *data;

		if (Existe(local))
		{
			if (mapeamento_imagems.pegar(local).get() == NULL)
			{
				data = stbi_load(local.c_str(), &X, &Y, &canais, canais);

				imagem image = imagem(X, Y, canais, data);
				image.local = local;
				// delete[] data;

				mapeamento_imagems.aplicar(local, image);

				escrever(local);
				escrever(mapeamento_imagems.pegar(local));

				return mapeamento_imagems.pegar(local);
			}
			else
			{
				return mapeamento_imagems.pegar(local);
			}
		}
		else
		{
			cout << "nao foi possivel carregar " << local << endl;
			shared_ptr<imagem> ret;
			return ret;
		}
	}

	void carregar_Imagem_thread(string local, shared_ptr<imagem> *ret)
	{
		*ret = carregar_Imagem(local);
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
	// remover pasta_imagems
	shared_ptr<tile_set> carregar_tile_set(string local)
	{
		string pasta_imagems = pegar_pasta_arquivo(local);
		if (Existe(local.c_str()))
		{
			if (mapeamento_tilesets.pegar(local).get() == NULL)
			{
				cout << "criando tile_set" << endl;
				// pegar info tile_set
				tile_set ret = tile_set();
				ret.local = local;
				ifstream arquivo(local);
				json j;
				arquivo >> j;

				if (j.find("image") != j.end())
				{
					ret.local_imagem = pasta_imagems + j["image"].get<string>();
					ret.tiles_img = carregar_Imagem(ret.local_imagem);
				}
				else
				{
					cout << "tile_set " << local << "n�o posui imagem" << endl;
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
							// else if (name.compare("colision_type") == 0) {
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
				else
				{
					cout << "nao tem tiles\n";
				}

				mapeamento_tilesets.aplicar(local, ret);

				return mapeamento_tilesets.pegar(local);
			}
			else
			{

				return mapeamento_tilesets.pegar(local);
			}
		}
		else
		{
			cout << "nao foi possivel carregar " << local << endl;
			shared_ptr<tile_set> ret;
			return ret;
		}
	}

	void carregar_tile_set_thread(string local, shared_ptr<tile_set> *ret)
	{
		*ret = carregar_tile_set(local);
	}

	mapeamento_assets<tile_map_info> mapeamento_tile_map_infos;

	shared_ptr<tile_map_info> carregar_info_tile_map(string local)
	{
		if (Existe(local.c_str()))
		{

			if (mapeamento_tile_map_infos.pegar(local).get() == NULL)
			{

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

				mapeamento_tile_map_infos.aplicar(local, ret);
				// cout << "quantidade camadas: " << ret->info.size() << endl;
				return mapeamento_tile_map_infos.pegar(local);
			}
			else
			{
				cout << "nao foi possivel carregar " << local << endl;
				shared_ptr<tile_map_info> ret = mapeamento_tile_map_infos.pegar(local);
				return ret;
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

	// https://github.com/Biendeo/Doom-WAD-Reader
	// https://github.com/id-Software/DOOM/tree/master/linuxdoom-1.10

	mapeamento_assets<cena_3D> cenas_3D;

	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

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
				ret.malhas.insert(m);
				ret.objetos.minhas_malhas.push_back(m.second);
			}

			// adicionar material
			ret.materiais.insert(pair<string, Material>("material", mat));
			ret.objetos.meus_materiais.push_back(mat);
			cenas_3D.aplicar(local, ret);
		}

		return cenas_3D.pegar(local);
	}

	void importar_obj_thread(string local, string local_mtl, shared_ptr<cena_3D> *ret)
	{
		*ret = importar_obj(local);
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

	void storeTableData(std::string filename, Table table)
	{
		json data = table_json(table);

		std::ofstream file(filename, std::ios::out | std::ios::binary);
		if (file.good())
		{
			std::vector<uint8_t> bson_data = json::to_bson(data);
			file.write((const char *)&bson_data[0], bson_data.size());
			file.close();
			std::cout << "Data saved to " << filename << std::endl;
		}
		else
		{
			std::cerr << "Error: could not save data to " << filename << std::endl;
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
		cena_3D ret;
		ret.nome = local;

		Full_Map_Info map_info = read_map_file(local);

		cenas_3D.aplicar(local, ret);
		return cenas_3D.pegar(local);
	}
	void importar_map_thread(string local, shared_ptr<cena_3D> *ret)
	{
		*ret = importar_map(local);
	}

	malha converter_malha_gltf(gltf_loader::Mesh m, string file_path)
	{
		malha ret;
		ret.indice = m.indices;
		ret.nome = m.name;
		ret.arquivo_origem = file_path;

		for (int i = 0; i < m.positions.size(); i++)
		{
			vertice v;

			v.posicao[0] = m.positions[i].x;
			v.posicao[1] = m.positions[i].y;
			v.posicao[2] = m.positions[i].z;

			v.normal[0] = m.normals[i].x;
			v.normal[1] = m.normals[i].y;
			v.normal[2] = m.normals[i].z;

			v.uv[0] = m.texcoords[i].x;
			v.uv[1] = m.texcoords[i].y;

			ret.vertices.push_back(v);
		}

		// print({"nome_malha",ret.nome,"numero de triangulos",ret.indice.size() / 3});
		// print({"nome_malha",ret.nome,"numero de vertices",ret.vertices.size()});

		return ret;
	}

	objeto_3D node_3D_object(gltf_loader::Node node, vector<gltf_loader::Node> full_node_list, cena_3D cena, gltf_loader::GLTFLoader loader)
	{
		objeto_3D ret;
		ret.nome = node.name;
		ret.id = node.id;
		// print({node.name});

		ret.posicao = node.translation;
		ret.quaternion = node.rotation;
		ret.escala = node.scale;

		for (int i = 0; i < node.meshIndices.size(); i++)
		{

			int mesh_index = node.meshIndices[i];
			string mesh_name = loader.meshes[mesh_index].name;
			ret.minhas_malhas.push_back(cena.malhas[mesh_name]);

			int material_index = loader.meshes[mesh_index].material;

			if (material_index <= loader.materials.size() - 1 && loader.materials.size() != 0)
			{
				string material_name = loader.materials[material_index].name;
				ret.meus_materiais.push_back(cena.materiais[material_name]);
			}
			else
			{
				Material mat;
				mat.shad = "resources/Shaders/mesh";
				mat.texturas[0] = carregar_Imagem("resources/Textures/white.png");
				ret.meus_materiais.push_back(mat);
			}
		}

		//correct position in objects with no nodes
		if(node.meshIndices.size() == 0){
			ret.posicao.x = -node.translation.x;
			ret.posicao.z = -node.translation.z;
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
		cena_3D ret;
		if (cenas_3D.pegar(local).get() == NULL)
		{

			gltf_loader::GLTFLoader gltf_loader(local);
			gltf_loader.load();

			for (int i = 0; i < gltf_loader.meshes.size(); i++)
			{
				ret.malhas.insert(pair<string, shared_ptr<malha>>(gltf_loader.meshes[i].name, make_shared<malha>(converter_malha_gltf(gltf_loader.meshes[i], local))));
			}

			for (int i = 0; i < gltf_loader.materials.size(); i++)
			{
				string image_location;
				if (gltf_loader.materials[i].textureIndex != 0)
				{
					image_location = pegar_pasta_arquivo(local) + gltf_loader.textures[gltf_loader.materials[i].textureIndex].uri;
				}
				Material mat;
				mat.shad = "resources/Shaders/mesh";
				mat.cor = gltf_loader.materials[i].baseColorFactor;
				mat.texturas[0] = ManuseioDados::carregar_Imagem(image_location);
				if (mat.texturas[0] == NULL)
				{
					mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");
				}
				ret.materiais[gltf_loader.materials[i].name] = mat;
			}

			for (int i = 0; i < gltf_loader.textures.size(); i++)
			{
				string arquivo_textura = pegar_pasta_arquivo(local) + gltf_loader.textures[i].uri;
				ret.texturas[arquivo_textura] = carregar_Imagem(arquivo_textura);
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

				// print({"ani.nome: ",a.name});

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
						//correct possition
						if(gltf_loader.nodes[okf.object_id].meshIndices.size() == 0){
							okf.position.x = -okf.position.x;
							okf.position.z = -okf.position.z;
						}
						okf.rotation = kf.rotation;
						okf.scale = kf.scale;

						okfs.push_back(okf);
					}
					ani.keyFrames.push_back(okfs);
				}

				ret.animacoes.insert(pair<string, animacao>(a.name, ani));
			}

			

			cenas_3D.aplicar(local, ret);
		}
		return cenas_3D.pegar(local);
	}
	void importar_gltf_thread(string local, shared_ptr<cena_3D> *ret)
	{
		*ret = importar_gltf(local);
	}

	// arquivo_origem
	map<string, shared_ptr<cena_3D> (*)(string)> funcoes_abrir_modelos_3D = {
		pair<string, shared_ptr<cena_3D> (*)(string)>(".obj", importar_obj),
		pair<string, shared_ptr<cena_3D> (*)(string)>(".map", importar_map),
		pair<string, shared_ptr<cena_3D> (*)(string)>(".gltf", importar_gltf),
	};
	shared_ptr<cena_3D> carregar_modelo_3D(string local)
	{
		return funcoes_abrir_modelos_3D[pegar_estencao_arquivo(local)](local);
	}
	void carregar_modelo_3D(string local, shared_ptr<cena_3D> *ret)
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

}
