#pragma once

#include "RecursosT.h"
#include "ManusearArquivos.h"
#include "LoopPrincipal.h"
#include "input.h"
#include "Tempo.h"
using namespace Tempo;
#include "scene.h"
#include "box_2d.h"
#include "audio_source.h"

#include "render_mesh.h"
#include "projetil.h"
#include "render_sprite.h"
#include "render_tilemap.h"
#include "bullet.h"
#include "ui_element.h"
#include "light.h"
#include "poly_mesh.h"
#include "navmesh.h"

#include "args.h"
#include "game_object.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

#include "table.h"
#include "table_conversors.h"

#include "benchmark.h"

#include "args.h"

#include "config_folder_path.h"

#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef USE_LUA_JIT
extern "C"
{
#include <luajit/lua.h>
#include <luajit/lualib.h>
#include <luajit/lauxlib.h>
#include "luajit/luajit.h"
}

#else
extern "C"
{
#include "lua/lua.hpp"
}

#endif

const int get_lua = 0;
const int set_lua = 1;

bool parallel_loading = false;

void load_base_lua_state(lua_State *L, string path);

bool isNumber(const std::string &str)
{
	std::istringstream iss(str);
	double num;
	iss >> std::noskipws >> num;
	return iss.eof() && !iss.fail();
}

float stringToFloat(const std::string &str)
{
	std::istringstream iss(str);
	float result;
	iss >> result;
	return result;
}

std::string floatToString(float value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

lua_State *lua_global_data;
void start_lua_global_data()
{
	print("iniciando lua global data");
	lua_global_data = luaL_newstate();

#ifdef USE_LUA_JIT
	luaJIT_setmode(lua_global_data, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
	lua_gc(lua_global_data, LUA_GCSETPAUSE, 100);
	lua_gc(lua_global_data, LUA_GCSETSTEPMUL, 400);
#else
	lua_gc(lua_global_data, LUA_GCSETSTEPMUL, 1000);
#endif
}

void lua_pushtable(lua_State *L, Table t)
{
	lua_newtable(L);
	for (std::pair<std::string, float> p : t.m_floatMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushnumber(L, p.second);
		lua_settable(L, -3);
	}
	for (std::pair<std::string, std::string> p : t.m_stringMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushstring(L, p.second.c_str());
		lua_settable(L, -3);
	}
	for (std::pair<std::string, lua_CFunction> p : t.m_lua_functionMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushcfunction(L, p.second);
		lua_settable(L, -3);
	}
	for (std::pair<std::string, Table> p : t.m_tableMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushtable(L, p.second);
		lua_settable(L, -3);
	}
}

Table lua_totable(lua_State *L, int index)
{
	Table t;

	// Iterate over the table and extract its keys and values
	lua_pushnil(L); // Push the first key
	while (lua_next(L, index) != 0)
	{
		std::string key;
		// At this point, the stack contains the key at index -2 and the value at index -1

		if (lua_type(L, -2) == LUA_TNUMBER)
		{
			// The value is a number
			float value = lua_tonumber(L, -2);
			key = floatToString(value);
		}
		else if (lua_type(L, -2) == LUA_TSTRING)
		{
			// The value is a number
			std::string value = lua_tostring(L, -2);
			key = value;
		}
		else if (lua_type(L, -2) == LUA_TBOOLEAN)
		{
			// The value is a number
			float value = lua_toboolean(L, -2);
			key = std::to_string(value);
		}

		if (lua_type(L, -1) == LUA_TNUMBER)
		{
			// The value is a number
			float value = lua_tonumber(L, -1);
			t.setFloat(key, value);
		}
		else if (lua_type(L, -1) == LUA_TSTRING)
		{
			// The value is a number
			std::string value = lua_tostring(L, -1);
			t.setString(key, value);
		}
		else if (lua_type(L, -1) == LUA_TBOOLEAN)
		{
			// The value is a number
			float value = lua_toboolean(L, -1);
			t.setFloat(key, value);
		}
		else if (lua_type(L, -1) == LUA_TFUNCTION)
		{
			// The value is a number
			lua_CFunction value = lua_tocfunction(L, -1);
			t.setLuaFunction(key, value);
		}
		else if (lua_type(L, -1) == LUA_TTABLE)
		{
			// The value is a table, recurse into it
			t.setTable(key, lua_totable(L, lua_gettop(L)));
		}
		// Pop the value, but leave the key for the next iteration
		lua_pop(L, 1);
	}
	return t;
}

std::wstring lua_towstring(lua_State *L, int index)
{
	const char *utf8Str = luaL_checkstring(L, index);

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(utf8Str);
}

void lua_pushwstring(lua_State *L, const std::wstring &ws)
{
	// Convert wstring to UTF-8 encoded string
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string utf8str = converter.to_bytes(ws);

	// Push the UTF-8 string to the Lua stack
	lua_pushstring(L, utf8str.c_str());
}

using lua_function = int (*)(lua_State *);

int register_function_set(lua_State *);

int writerFunction(lua_State *, const void *p, size_t sz, void *ud)
{
	std::stringstream *ss = static_cast<std::stringstream *>(ud);
	ss->write(static_cast<const char *>(p), sz);
	return 0;
}

std::string compileLuaFile(std::string path)
{

	std::replace(path.begin(), path.end(), '.', '/');

	string filename = string("resources/Scripts/") + path + string(".lua");

	std::ifstream file(filename, std::ios::in | std::ios::binary);

	if (filename != "")
	{

		if (!file)
		{
			std::cerr << "Error: Failed to open Lua file " << filename << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();

		std::string luaScript = buffer.str();

		lua_State *L = luaL_newstate();

		lua_register(L, "register_function_set", register_function_set);

		if (luaL_loadstring(L, luaScript.c_str()) != 0)
		{
			std::cerr << "Error: Failed to compile Lua script" << std::endl;
			lua_close(L);
			return "";
		}

		std::stringstream compiledScript;

#ifdef USE_LUA_JIT

		if (lua_dump(L, writerFunction, &compiledScript) != 0)
		{
			std::cerr << "Error: Failed to dump compiled Lua script" << std::endl;
			lua_close(L);
			return "";
		}

#else

		if (lua_dump(L, writerFunction, &compiledScript, 0) != 0)
		{
			std::cerr << "Error: Failed to dump compiled Lua script" << std::endl;
			lua_close(L);
			return "";
		}

#endif

		lua_close(L);

		return compiledScript.str();
	}

	return "";
}

set<string> lua_scripts_loading_requests_files;
mapeamento_assets<string> mapeamento_scripts_lua;
std::mutex scripts_mtx;

shared_ptr<string> carregar_script_lua(string local)
{
	std::lock_guard<std::mutex> lock(scripts_mtx);
	if (lua_scripts_loading_requests_files.find(local) == lua_scripts_loading_requests_files.end())
	{
		if (mapeamento_scripts_lua.pegar(local) == NULL)
		{
			mapeamento_scripts_lua.aplicar(local, compileLuaFile(local));
		}
	}
	return mapeamento_scripts_lua.pegar(local);
}

namespace lua_debug
{

	size_t get_string_size(lua_State *L, int index)
	{
		if (lua_type(L, index) == LUA_TSTRING)
		{
			const char *str = lua_tostring(L, index);
			if (str)
			{
				return strlen(str);
			}
		}
		return 0;
	}

	size_t get_table_size(lua_State *L, int index)
	{
		if (lua_type(L, index) == LUA_TTABLE)
		{
			size_t size = 0;
			lua_pushnil(L);
			while (lua_next(L, index) != 0)
			{
				size += get_string_size(L, -1);
				lua_pop(L, 1);
			}
			return size;
		}
		return 0;
	}

	/**/
	int compare_variable_memory(lua_State *L)
	{
		int stack_size = lua_gettop(L);

		size_t largest_variable_size = 0;
		int largest_variable_index = -1;
		const char *largest_variable_name = NULL;

		for (int i = 1; i <= stack_size; i++)
		{
			// size_t variable_size = lua_gc(L, LUA_GCCOUNT, 0);
			size_t variable_size = get_string_size(L, i) + get_table_size(L, i);

			if (variable_size > largest_variable_size)
			{
				largest_variable_size = variable_size;
				largest_variable_index = i;

				// Verifique se a variável é uma tabela com uma chave
				if (lua_istable(L, i))
				{
					lua_pushnil(L);
					if (lua_next(L, i) != 0)
					{
						largest_variable_name = lua_tostring(L, -2);
						lua_pop(L, 2); // Remova a chave e o valor da pilha
					}
				}
			}
		}

		if (largest_variable_index > 0)
		{
			const char *variable_type = lua_typename(L, lua_type(L, largest_variable_index));
			printf("A variável mais consumidora de memória é do tipo %s na posição %d e consome %zu bytes.\n", variable_type, largest_variable_index, largest_variable_size);

			if (largest_variable_name)
			{
				printf("Nome ou chave da variável: %s\n", largest_variable_name);
			}
			else
			{
				printf("A variável não tem um nome ou chave identificável.\n");
			}
		}
		else
		{
			printf("Nenhuma variável na pilha.\n");
		}

		return 0;
	}
};

// aqui

int add_component(lua_State *L);
int remove_component(lua_State *L);
int get_component_size(lua_State *L);
int have_component(lua_State *L);

// scripts
int get_script_size(lua_State *L);
int set_script_var(lua_State *L);

int get_lua_component(lua_State *L);
int have_script(lua_State *L);
int add_script_lua(lua_State *L);
int remove_script(lua_State *L);

int get_lua_var(lua_State *L);
int set_lua_var(lua_State *L);
int call_lua_function(lua_State *L);

// banchmark
int make_banchmark(lua_State *L);

// lambda labs

void change_pos(objeto_jogo *obj, vec3 pos)
{
	shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
	shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
	shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
	if (tf != NULL)
	{
		tf->pos = pos;
	}

	if (bu != NULL)
	{
		bu->mudar_pos(pos);
	}
	else if (b2d != NULL)
	{
		b2d->mudar_pos(vec2(pos.x, pos.y));
	}
};

void change_rot(objeto_jogo *obj, vec3 rot)
{
	shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
	if (tf != NULL)
	{
		tf->mudar_angulo_graus(rot);
	}
	shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
	if (b2d != NULL)
	{
		b2d->mudar_rot(rot.x);
	}
	shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
	if (bu != NULL)
	{
		bu->mudar_rot(rot);
	}
};

key_frame mix_keyframe(key_frame a, key_frame b, float time)
{
	if (a.has_position)
	{
		a.position = mix(a.position, b.position, time);
		return a;
	}
	else if (a.has_rotation)
	{
		a.rotation = mix(a.rotation, b.rotation, time);
		return a;
	}
	else if (a.has_scale)
	{
		a.rotation = mix(a.scale, b.scale, time);
		return a;
	}
	return a;
};

bool key_frame_match(key_frame a, key_frame b)
{
	if (a.object_id == b.object_id && (a.has_position == b.has_position || a.has_rotation == b.has_rotation || a.has_scale == b.has_scale))
	{
		return true;
	}
	return false;
}

vector<key_frame> mix_keyframes(vector<key_frame> a, vector<key_frame> b, float time)
{

	for (int x = 0; x < a.size(); x++)
	{
		key_frame kf_a = a[x];

		for (int y = 0; y < b.size(); y++)
		{
			key_frame kf_b = b[y];
			if (key_frame_match(kf_a, kf_b))
			{

				// fazer coisa
				if (kf_a.has_position)
				{
					a[x].position = mix(a[x].position, b[y].position, time);
				}
				else if (kf_a.has_rotation)
				{
					a[x].rotation = mix(a[x].rotation, b[y].rotation, time);
				}
				else if (kf_a.has_scale)
				{
					a[x].scale = mix(a[x].scale, b[y].scale, time);
				}

				b.erase(b.begin() + y);
				break;
			}
		}
	}

	return a;
};

void apply_key_frame_transform(std::vector<key_frame> key_frames, vector<objeto_jogo *> objects_ptrs, map<size_t, mat4> offset_matrices)
{

	for (objeto_jogo *obj : objects_ptrs)
	{
		shared_ptr<render_malha> rm = obj->pegar_componente<render_malha>();
		if (rm != NULL)
		{
			rm->bones = objects_ptrs;

			if (rm->bones.size() > 0)
			{
				rm->bones = objects_ptrs;
			}
		}
	}

	// apply offset_matrix
	/**/
	for (size_t i = 0; i < objects_ptrs.size(); i++)
	{

		shared_ptr<transform_> tf = objects_ptrs[i]->pegar_componente<transform_>();
		if (tf != NULL && offset_matrices.find(i) != offset_matrices.end())
		{
			tf->offset_matrix = offset_matrices[i];
		}
	}

	for (key_frame kfs : key_frames)
	{
		if (kfs.has_position)
		{
			change_pos(objects_ptrs[kfs.object_id], kfs.position);
		}
		else if (kfs.has_rotation)
		{
			change_rot(objects_ptrs[kfs.object_id], quat_graus(kfs.rotation));
		}
		else if (kfs.has_scale)
		{
			objects_ptrs[kfs.object_id]->pegar_componente<transform_>()->esca = kfs.scale;
		}
	}
};

namespace funcoes_ponte
{

	// exemplos get set
	/*
	int get_set_example(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;

			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			return 0;
		}
	}
	int get_set_transform(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));

			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));

			return 0;
		}
	}
	*/

	// screen

	int get_config_folder_path(lua_State *L)
	{
		lua_pushstring(L, config_folder_path().c_str());
		return 1;
	}

	// Função em C para criar um diretório
	int create_directory(lua_State *L)
	{
		const char *diretorio = luaL_checkstring(L, 1);

#ifdef _WIN32
		if (_mkdir(diretorio) == 0)
		{
			lua_pushboolean(L, 1);
		}
		else
		{
			lua_pushboolean(L, 0);
		}
#endif
#ifdef __unix__
		if (mkdir(diretorio, S_IRWXU | S_IRWXG | S_IRWXO) == 0)
		{
			lua_pushboolean(L, 1);
		}
		else
		{
			lua_pushboolean(L, 0);
		}
#endif

		return 1; // Número de valores de retorno
	}

	int get_set_window(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			ret.setTable("resolution", vec2_table(gerente_janela->pegar_res()));
			ret.setFloat("full_screen", gerente_janela->e_janela_cheia());
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			Table res = t.getTable("resolution");
			loop_principal::mudar_res((int)res.getFloat("x"), (int)res.getFloat("y"));
			api_grafica->res_interna = ivec2((int)res.getFloat("x"), (int)res.getFloat("y"));
			loop_principal::setar_tela_inteira_como((bool)t.getFloat("full_screen"));
			api_grafica->update_res = true;

			return 0;
		}
	}

	int enable_cursor(lua_State *L)
	{
		if (lua_toboolean(L, 1))
		{
			gerente_janela->enable_cursor(true);
		}
		else
		{
			gerente_janela->enable_cursor(false);
		}

		return 0;
	}

	int set_cursor_image(lua_State *L)
	{

		if (lua_gettop(L) == 0)
		{
			gerente_janela->mudar_cursor(NULL);
		}
		else
		{
			gerente_janela->mudar_cursor(ManuseioDados::carregar_Imagem(lua_tostring(L, 1)));
		}

		return 0;
	}

	int close(lua_State *L)
	{
		gerente_janela->fechar = true;
		return 0;
	}

	// time
	int get_time(lua_State *L)
	{

		Table ret;
		ret.setFloat("time", Tempo::tempo);
		ret.setFloat("delta", deltaTimer.get());
		// ret.setFloat("delta", time_step);
		ret.setFloat("scale", Tempo::velocidadeTempo);
		lua_pushtable(L, ret);
		return 1;
	}

	int set_time_scale(lua_State *L)
	{
		Tempo::velocidadeTempo = lua_tonumber(L, 1);
		// print("Tempo::velocidadeTempo",Tempo::velocidadeTempo);
		return 0;
	}

	// object
	int create_object(lua_State *L)
	{

		int argumentos = lua_gettop(L);
		shared_ptr<objeto_jogo> obj = novo_objeto_jogo();
		if (argumentos == 0)
		{
			cena_objetos_selecionados->adicionar_objeto(obj);
		}
		else if (argumentos == 1)
		{
			cena_objetos_selecionados->adicionar_objeto(string_ponteiro<objeto_jogo>(lua_tostring(L, 1)), obj);
		}

		lua_pushstring(L, ponteiro_string<objeto_jogo>(obj.get()).c_str());
		return 1;
	}

	int get_object_with_name(lua_State *L)
	{
		string output = "";
		shared_ptr<objeto_jogo> obj = (*cena_objetos_selecionados)[lua_tostring(L, 1)];
		output = ponteiro_string(obj.get());
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int remove_object(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos == 1)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
			obj->excluir();
		}

		return 0;
	}

	int reset_components(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
			obj->limpar_componentes();
		}
		return 0;
	}

	/*
	int get_object_family(lua_State *L)
	{
		Table ret;
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo *obj = NULL;
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));

		ret.setString("father", ponteiro_string(obj->pai));

		vector<string> criancas;
		for (shared_ptr<objeto_jogo> p : obj->filhos)
		{
			criancas.push_back(ponteiro_string(p.get()));
		}

		ret.setTable("childrens", vString_table(criancas));
		lua_pushtable(L, ret);
		return 1;
	}
	*/

	int get_set_object(lua_State *L)
	{

		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo *obj = NULL;
		int get_set = lua_tonumber(L, 1);

		if (get_set == get_lua)
		{

			Table ret;

			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));

			ret.setString("name", obj->nome.c_str());

			ret.setString("father", ponteiro_string(obj->pai));

			vector<string> criancas;
			for (shared_ptr<objeto_jogo> p : obj->filhos)
			{
				criancas.push_back(ponteiro_string(p.get()));
			}

			ret.setTable("childrens", vString_table(criancas));
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);

			obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));

			obj->nome = t.getString("name");

			vector<string> criancas = table_vString(t.getTable("childrens"));
			obj->filhos = {};
			for (string s : criancas)
			{
				obj->filhos.push_back(string_ponteiro<objeto_jogo>(s)->get_this_object());
			}

			obj->pai = string_ponteiro<objeto_jogo>(t.getString("father"));

			return 0;
		}
	}

	// memory
	int clear_memory(lua_State *L)
	{
		buffers_som.limpar_lixo();
		ManuseioDados::mapeamento_fontes.limpar_lixo();
		ManuseioDados::mapeamento_imagems.limpar_lixo();
		ManuseioDados::mapeamento_tilesets.limpar_lixo();
		ManuseioDados::mapeamento_tile_map_infos.limpar_lixo();
		ManuseioDados::cenas_3D.limpar_lixo();
		mapeamento_scripts_lua.limpar_lixo();
		clean_scene_3D_table_cache();
		return 0;
	}

	// arquivos

	int is_loaded(lua_State *L)
	{
		int asset_type = lua_tonumber(L, 1);
		string file_path = lua_tostring(L, 2);
		bool load = lua_toboolean(L, 3);
		bool ret = false;

		if (asset_type == 1)
		{
			ret = ManuseioDados::mapeamento_imagems.pegar(file_path) != NULL;
			if (!ret && load && !ManuseioDados::has_loading_request(file_path))
			{
				thread loader(ManuseioDados::carregar_Imagem, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 2)
		{
			ret = ManuseioDados::mapeamento_fontes.pegar(file_path) != NULL;
			if (!ret && load && !ManuseioDados::has_loading_request(file_path))
			{
				thread loader(ManuseioDados::carregar_fonte, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 3)
		{
			ret = ManuseioDados::mapeamento_tilesets.pegar(file_path) != NULL;
			if (!ret && load && !ManuseioDados::has_loading_request(file_path))
			{
				thread loader(ManuseioDados::carregar_tile_set, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 4)
		{
			ret = ManuseioDados::mapeamento_tile_map_infos.pegar(file_path) != NULL;
			if (!ret && load && !ManuseioDados::has_loading_request(file_path))
			{
				thread loader(ManuseioDados::carregar_info_tile_map, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 5)
		{
			ret = ManuseioDados::cenas_3D.pegar(file_path) != NULL;
			if (!ret && load && !ManuseioDados::has_loading_request(file_path))
			{
				auto load_3D_model = [=]()
				{
					register_scene_3D_table(ManuseioDados::carregar_modelo_3D(file_path));
				};
				thread loader(load_3D_model);
				loader.detach();
			}
		}
		else if (asset_type == 6)
		{
			ret = buffers_som.pegar(file_path) != NULL;
			if (!ret && load && audio_source_loading_requests_files.find(file_path) == audio_source_loading_requests_files.end())
			{
				thread loader(carregar_audio_buffer, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 7)
		{
			ret = mapeamento_scripts_lua.pegar(file_path) != NULL;
			if (!ret && load && lua_scripts_loading_requests_files.find(file_path) == lua_scripts_loading_requests_files.end())
			{
				thread loader(carregar_script_lua, file_path);
				loader.detach();
			}
		}

		//

		lua_pushboolean(L, ret);
		return 1;
	}

	int memory_usage_info(lua_State *L)
	{
		print("KBs usage", lua_gc(L, LUA_GCCOUNTB, 0));
		// put the code here
		return 0;
	}

	// input

	int set_cursor_position(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		if (argumentos == 2)
		{
			manuseio_inputs->set_mouse_position(lua_tonumber(L, 1), lua_tonumber(L, 2));
		}
		return 0;
	}

	int set_keyboard_text_input(lua_State *L)
	{
		manuseio_inputs->set_text_input(lua_toboolean(L, 1));
		return 0;
	}

	int get_keyboard_text_input(lua_State *L)
	{
		lua_pushfstring(L, manuseio_inputs->text_input.c_str());
		return 1;
	}

	int get_input(lua_State *L)
	{
		float ret = 0;

		int device = lua_tonumber(L, 1);
		int joystick_no = lua_tonumber(L, 2);
		string key = lua_tostring(L, 3);

		if (device == 1)
		{

			manuseio_inputs->get_text_input();

			teclado t = manuseio_inputs->keyboard_input;

			if (t.teclas.find(key) != t.teclas.end())
			{
				ret = t.teclas[key];
			}
		}
		else if (device == 2)
		{

			if (joystick_no <= manuseio_inputs->joysticks_input.size())
			{
				manuseio_inputs->get_joysticks_input();

				joystick j = manuseio_inputs->joysticks_input[joystick_no - 1];

				if (j.inputs.find(key) != j.inputs.end())
				{
					ret = j.inputs[key];
				}
			}
		}
		else if (device == 3)
		{
			input_mouse m = manuseio_inputs->mouse_input;

			if (m.botoes.find(key) != m.botoes.end())
			{
				ret = m.botoes[key];
			}
			else if (m.movimentos.find(key) != m.movimentos.end())
			{
				ret = m.movimentos[key];
			}
		}

		lua_pushnumber(L, ret);
		return 1;
	}

	// char control

	int to_move(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj != NULL)
		{
			if (argumentos == 3)
			{
				shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
				if (b2d != NULL)
				{
					b2d->mover(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
				}
			}
		}
		return 0;
	}

	int in_ceiling(lua_State *L)
	{
		bool output = false;
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj->pegar_componente<fisica_char_B2D>() != NULL)
		{
			shared_ptr<fisica_char_B2D> fc = obj->pegar_componente<fisica_char_B2D>();
			if (fc != NULL)
			{
				output = fc->teto;
			}
		}

		lua_pushboolean(L, output);
		return 1;
	}

	int in_floor(lua_State *L)
	{
		bool output = false;
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj->pegar_componente<fisica_char_B2D>() != NULL)
		{
			shared_ptr<fisica_char_B2D> fc = obj->pegar_componente<fisica_char_B2D>();
			if (fc != NULL)
			{
				output = fc->chao;
			}
		}

		lua_pushboolean(L, output);
		return 1;
	}

	// gravity

	int get_gravity(lua_State *L)
	{
		lua_pushnumber(L, gravidade.x);
		lua_pushnumber(L, gravidade.y);
		lua_pushnumber(L, gravidade.z);
		return 3;
	}

	int set_gravity(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		if (argumentos == 3)
		{
			gravidade = vec3(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
		}
		return 0;
	}

	// transform
	int get_set_transform(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
			ret.setFloat("is_ui", tf->UI);
			ret.setTable("position", vec3_table(tf->pos));
			ret.setTable("rotation", vec3_table(tf->pegar_angulo_graus()));
			ret.setTable("scale", vec3_table(tf->esca));
			ret.setFloat("billboarding", tf->billboarding);

			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
			if (tf != NULL)
			{
				tf->UI = t.getFloat("is_ui");
				tf->pos = table_vec3(t.getTable("position"));
				tf->mudar_angulo_graus(table_vec3(t.getTable("rotation")));
				tf->esca = table_vec3(t.getTable("scale"));
				tf->billboarding = t.getFloat("billboarding");
				tf->pegar_matriz();
			}

			return 0;
		}
	}

	int move_transform(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->pos += v3;
		}
		return 0;
	}

	

	int rotate_transform(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{

			vec3 v3a = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			vec3 v3b = tf->pegar_angulo_graus();
			vec3 v3c = vec3(v3a.x + v3b.x, v3a.y + v3b.y, v3a.z + v3b.z);
			tf->mudar_angulo_graus(v3c);
		}
		return 0;
	}

	int change_transfotm_position(lua_State *L)
	{

		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
		change_pos(obj, v3);

		return 0;
	}

	int change_transfotm_rotation(lua_State *L)
	{

		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
		change_rot(obj, v3);
		return 0;
	}

	int change_transfotm_scale(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->esca = v3;
		}
		return 0;
	}

	int get_local_direction_transform(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		Table ret;
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{
			vec3 dir = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			ret = vec3_table(tf->pegar_direcao_local(dir));
		}
		lua_pushtable(L, ret);
		return 1;
	}

	int get_global_position_transform(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		Table ret;
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{
			vec3 dir = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			ret = vec3_table(tf->pegar_direcao_global(dir));
		}
		lua_pushtable(L, ret);
		return 1;
	}

	int get_translation_position_transform(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		Table ret;
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{
			vec3 dir = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			ret = vec3_table(tf->mover_direcao(dir));
		}
		lua_pushtable(L, ret);
		return 1;
	}

	int transform_look_at(lua_State *L)
	{
		
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();

		vec3 up_axe = table_vec3(lua_totable(L, 2));
		vec3 target_position = table_vec3(lua_totable(L, 3));
		bool look_up = lua_tonumber(L, 4);

		if(look_up){
			vec3 rot = tf->billboarding_spherical(target_position);
			change_rot(obj, rot);
		}else{
			vec3 rot = tf->billboarding_planar(target_position);
			change_rot(obj, rot);
		}
		

		return 0;
	}

	// sprite render

	int get_set_sprite_render(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_sprite> rs = obj->pegar_componente<render_sprite>();
			ret.setFloat("layer", rs->camada + 1);
			ret.setFloat("selected_tile", rs->tile_selecionado);
			ret.setString("tile_set_local", rs->tiles->local);
			ret.setTable("material", material_table(rs->mat));
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_sprite> rs = obj->pegar_componente<render_sprite>();
			rs->camada = t.getFloat("layer") - 1;
			rs->tile_selecionado = t.getFloat("selected_tile");
			rs->tiles = ManuseioDados::carregar_tile_set(t.getString("tile_set_local"));
			rs->mat = table_material(t.getTable("material"));
			return 0;
		}
	}

	// render tilemap

	int get_tilemap(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL && obj->pegar_componente<render_tilemap>()->map_info != NULL)
		{
			output = obj->pegar_componente<render_tilemap>()->map_info->local;
		}
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int set_tilemap(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL)
		{
			ManuseioDados::carregar_info_tile_map_thread(lua_tostring(L, 2), &obj->pegar_componente<render_tilemap>()->map_info);
			// obj->pegar_componente<render_tilemap>()->map_info = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 2));
		}
		return 0;
	}

	int get_render_only_tilemap_layer(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		int output = -1;
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL)
		{
			output = obj->pegar_componente<render_tilemap>()->apenas_camada;
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int set_render_only_tilemap_layer(lua_State *L)
	{
		int argumentos = lua_gettop(L);

		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL)
		{
			obj->pegar_componente<render_tilemap>()->apenas_camada = lua_tonumber(L, 2);
		}
		return 0;
	}

	int get_set_render_tilemap(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
			ret.setFloat("layer", rtm->camada + 1);
			ret.setTable("material", material_table(rtm->mat));
			ret.setFloat("render_tilemap_only_layer", rtm->apenas_camada);
			ret.setString("tile_set_local", rtm->tiles->local);
			ret.setString("tile_map_local", rtm->map_info->local);
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
			rtm->camada = t.getFloat("layer") - 1;
			rtm->mat = table_material(t.getTable("material"));
			rtm->apenas_camada = t.getFloat("render_tilemap_only_layer");
			rtm->tiles = ManuseioDados::carregar_tile_set(t.getString("tile_set_local"));
			rtm->map_info = ManuseioDados::carregar_info_tile_map(t.getString("tile_map_local"));
			return 0;
		}
	}

	// texto
	int get_set_render_text(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
			ret.setFloat("layer", rt->camada + 1);
			ret.setString("font", rt->font->path);
			ret.setFloat("line_size", rt->tamanho_max_linha);
			ret.setFloat("uniform_space_between_characters", rt->uniform_space_between_characters);
			ret.setTable("material", material_table(rt->mat));
			std::wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
			ret.setString("text", converter.to_bytes(rt->texto));
			ret.setFloat("text_location_x", (char)rt->text_location_x);
			ret.setFloat("text_location_y", (char)rt->text_location_y);
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
			rt->camada = t.getFloat("layer") - 1;
			rt->font = ManuseioDados::carregar_fonte(t.getString("font"));
			rt->set_text_by_string(t.getString("text"));
			rt->tamanho_max_linha = t.getFloat("line_size");
			rt->uniform_space_between_characters = t.getFloat("uniform_space_between_characters");
			rt->mat = table_material(t.getTable("material"));
			rt->text_location_x = (char)t.getFloat("text_location_x");
			rt->text_location_y = (char)t.getFloat("text_location_y");
			return 0;
		}
	}

	int get_text_size(lua_State *L)
	{
		objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
		shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
		lua_pushtable(L, vec2_table(rt->get_text_size()));
		return 1;
	}

	int get_set_render_shader(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_shader> rt = obj->pegar_componente<render_shader>();
			ret.setFloat("layer", rt->camada + 1);
			ret.setFloat("vertex_size", rt->tamanho);
			ret.setTable("material", material_table(rt->mat));
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_shader> rt = obj->pegar_componente<render_shader>();
			rt->camada = t.getFloat("layer") - 1;
			rt->tamanho = t.getFloat("vertex_size");
			rt->mat = table_material(t.getTable("material"));
			return 0;
		}
	}

	// fisica

	int add_force(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3 && obj != NULL)
		{
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL)
			{
				b2d->adicionar_forca(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
			}
		}
		if (argumentos == 4 && obj != NULL)
		{
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
			if (bu != NULL)
			{
				bu->adicionar_forca(vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));
			}
		}
		return 0;
	}

	int add_impulse(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3 && obj != NULL)
		{
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL)
			{
				b2d->adicionar_impulso(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
			}
		}
		if (argumentos == 4 && obj != NULL)
		{
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
			if (bu != NULL)
			{
				bu->adicionar_impulso(vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));
			}
		}
		return 0;
	}

	int set_linear_velocity(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3 && obj != NULL)
		{
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL)
			{
				b2d->adicionar_velocidade(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
			}
		}
		if (argumentos == 4 && obj != NULL)
		{
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
			if (bu != NULL)
			{
				bu->adicionar_velocidade(vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));
			}
		}
		return 0;
	}

	int set_angular_velocity(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL)
		{
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL)
			{
				b2d->aplicar_velocidade_rotativa(lua_tonumber(L, 2));
			}
		}
		if (argumentos == 4 && obj != NULL)
		{
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
			if (bu != NULL)
			{
				bu->aplicar_velocidade_rotativa(vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));
			}
		}
		return 0;
	}

	int add_rotative_force(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL)
		{
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL)
			{
				b2d->adicionar_forca_rotativo(lua_tonumber(L, 2));
			}
		}
		if (argumentos == 4 && obj != NULL)
		{
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
			if (bu != NULL)
			{
				bu->adicionar_forca_rotativo(vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));
			}
		}
		return 0;
	}

	int add_rotative_impulse(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		objeto_jogo *obj = NULL;
		if (argumentos > 0)
		{
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL)
		{
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL)
			{
				b2d->adicionar_impulso_rotativo(lua_tonumber(L, 2));
			}
		}
		if (argumentos == 4 && obj != NULL)
		{
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
			if (bu != NULL)
			{
				bu->adicionar_impulso_rotativo(vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));
			}
		}
		return 0;
	}

	int get_set_physic_2D(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			ret.setTable("scale", vec2_table(b2d->escala));
			ret.setFloat("boady_dynamic", b2d->dinamica);
			ret.setFloat("collision_shape", b2d->forma);
			ret.setFloat("rotate", b2d->rotacionar);
			ret.setFloat("triger", b2d->gatilho);
			ret.setFloat("friction", b2d->atrito);
			ret.setFloat("density", b2d->densidade);
			ret.setFloat("gravity_scale", b2d->escala_gravidade);
			ret.setTable("collision_layer", info_camada_table(b2d->camada));
			ret.setFloat("elasticity", b2d->elasticidade);
			vector<Table> vertex;
			for (vec2 v2 : b2d->vertices)
			{
				vertex.push_back(vec2_table(v2));
			}
			ret.setTable("vertex", vTable_table(vertex));

			set<string> set_objs_touching;

			vector<Table> colis_infos;
			for (colis_info ci : b2d->colis_infos)
			{
				colis_infos.push_back(colis_info_table(ci));
				set_objs_touching.insert(ponteiro_string(ci.cos_obj));
			}

			vector<string> objs_touching(set_objs_touching.begin(), set_objs_touching.end());

			ret.setTable("objs_touching", vString_table(objs_touching));

			ret.setTable("colis_infos", vTable_table(colis_infos));
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			b2d->escala = table_vec2(t.getTable("scale"));
			b2d->dinamica = t.getFloat("boady_dynamic");
			b2d->forma = t.getFloat("collision_shape");
			b2d->rotacionar = t.getFloat("rotate");
			b2d->gatilho = t.getFloat("triger");
			b2d->atrito = t.getFloat("friction");
			b2d->densidade = t.getFloat("density");
			b2d->escala_gravidade = t.getFloat("gravity_scale");
			b2d->camada = table_info_camada(t.getTable("collision_layer"));
			b2d->elasticidade = t.getFloat("elasticity");
			vector<vec2> vertex;
			for (Table tvec2 : table_vTable(t.getTable("vertex")))
			{
				vertex.push_back(table_vec2(tvec2));
			}
			b2d->vertices = vertex;
			b2d->aplicar();
			return 0;
		}
	}

	// audio
	int get_set_audio(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<audio_source> au = obj->pegar_componente<audio_source>();
			ret.setString("path", au->info.nome);
			ret.setFloat("pause", au->info.pausa);
			ret.setFloat("loop", au->info.loop);
			ret.setFloat("time", au->info.tempo);
			ret.setFloat("speed", au->info.velocidade);
			ret.setFloat("volume", au->info.volume);
			ret.setFloat("min_distance", au->info.min_distance);
			ret.setFloat("atenuation", au->info.atenuation);
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<audio_source> au = obj->pegar_componente<audio_source>();

			au->info.nome = t.getString("path");
			au->info.pausa = t.getFloat("pause");
			au->info.loop = t.getFloat("loop");
			au->info.tempo = t.getFloat("time");
			au->info.velocidade = t.getFloat("speed");
			au->info.volume = t.getFloat("volume");
			au->info.min_distance = t.getFloat("min_distance");
			au->info.atenuation = t.getFloat("atenuation");
			au->aplicar_info();
			return 0;
		}
	}

	int get_set_global_volume(lua_State *L)
	{
		if (lua_gettop(L) > 0)
		{
			get_set_global_volume_sfml(lua_tonumber(L, 1));
			return 0;
		}
		else
		{
			lua_pushnumber(L, global_volume_sfml);
			return 1;
		}
	}

	int set_lisener_object(lua_State *L)
	{
		int argumentos = lua_gettop(L);
		audio_info output;
		if (argumentos > 0)
		{
			listener_transform = string_ponteiro<objeto_jogo>(lua_tostring(L, 1))->pegar_componente<transform_>();
		}
		else
		{
			listener_transform = NULL;
		}

		return 0;
	}
	// geral render

	int test_new_ui(lua_State *L)
	{

		if (lua_tostring(L, 2))
		{
			LuaFunctionWrapper lw;
			lw.L = L;
			lw.functionRef = lua_tostring(L, 2);
			test_ui(string_ponteiro<objeto_jogo>(lua_tostring(L, 1)), lw);
		}

		//,L,lua_tointeger(L, 2));
		return 0;
	}

	// post_processing
	int get_set_post_processing(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			lua_pushtable(L, material_table(api_grafica->pos_processamento_info));
			vec4 cor = api_grafica->pos_processamento_info.cor;
			print("color", cor.x, cor.y, cor.z, cor.w);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			api_grafica->pos_processamento_info = table_material(t);
			// cor
			vec4 cor = api_grafica->pos_processamento_info.cor;
			print("color", cor.x, cor.y, cor.z, cor.w);
			return 0;
		}
	}

	// render layers

	int get_set_render_layer_instruction(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			vector<Table> vt;
			for (instrucoes_render ir : api_grafica->info_render)
			{
				vt.push_back(table_instrucoes_render(ir));
			}
			lua_pushtable(L, vTable_table(vt));
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			vector<Table> vt = table_vTable(t);
			vector<instrucoes_render> irs;
			for (Table t2 : vt)
			{
				irs.push_back(table_instrucoes_render(t2));
			}
			api_grafica->info_render.clear();
			api_grafica->info_render = irs;
			return 0;
		}
	}

	int get_set_render_mesh(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_malha> mesh = obj->pegar_componente<render_malha>();
			ret.setFloat("layer", mesh->camada + 1);
			ret.setFloat("use_oclusion", mesh->usar_oclusao);
			vector<Table> meshes;
			for (shared_ptr<malha> m : mesh->malhas)
			{
				Table mesh_info;
				mesh_info.setString("file", m->arquivo_origem);
				mesh_info.setString("name", m->nome);
				meshes.push_back(mesh_info);
			}
			ret.setTable("meshes", vTable_table(meshes));
			vector<Table> materials;
			for (Material m : mesh->mats)
			{
				materials.push_back(material_table(m));
			}
			ret.setTable("materials", vTable_table(materials));
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{

			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_malha> mesh = obj->pegar_componente<render_malha>();
			mesh->camada = t.getFloat("layer") - 1;
			mesh->usar_oclusao = t.getFloat("use_oclusion");
			vector<Table> vt = table_vTable(t.getTable("meshes"));
			vector<shared_ptr<malha>> meshes;
			for (Table mesh : vt)
			{
				meshes.push_back(ManuseioDados::carregar_malha(mesh.getString("file"), mesh.getString("name")));
			}
			mesh->malhas = meshes;
			vt = table_vTable(t.getTable("materials"));
			vector<Material> materials;
			for (Table mat : vt)
			{
				materials.push_back(table_material(mat));
			}
			mesh->mats = materials;
			return 0;
		}
	}

	int get_set_render_poly_mesh(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<poly_mesh> mesh = obj->pegar_componente<poly_mesh>();
			ret.setFloat("layer", mesh->camada + 1);
			ret.setFloat("use_oclusion", mesh->usar_oclusao);
			vector<Table> meshes;
			for (shared_ptr<malha> m : mesh->malhas)
			{
				Table mesh_info;
				mesh_info.setString("file", m->arquivo_origem);
				mesh_info.setString("name", m->nome);
				meshes.push_back(mesh_info);
			}
			ret.setTable("meshes", vTable_table(meshes));
			vector<Table> materials;
			for (Material m : mesh->mats)
			{
				materials.push_back(material_table(m));
			}
			ret.setTable("materials", vTable_table(materials));

			ret.setTable("objects", vTable_table(meshes));
			vector<string> objects;
			for (shared_ptr<transform_> tf : mesh->transforms)
			{
				objects.push_back(ponteiro_string(tf->esse_objeto.get()));
			}
			ret.setTable("objects", vString_table(objects));
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{

			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<poly_mesh> mesh = obj->pegar_componente<poly_mesh>();
			mesh->camada = t.getFloat("layer") - 1;
			mesh->usar_oclusao = t.getFloat("use_oclusion");
			vector<Table> vt = table_vTable(t.getTable("meshes"));
			vector<shared_ptr<malha>> meshes;
			for (Table mesh : vt)
			{
				meshes.push_back(ManuseioDados::carregar_malha(mesh.getString("file"), mesh.getString("name")));
			}
			mesh->malhas = meshes;
			vt = table_vTable(t.getTable("materials"));
			vector<Material> materials;
			for (Table mat : vt)
			{
				materials.push_back(table_material(mat));
			}
			mesh->mats = materials;
			vector<shared_ptr<transform_>> objects;
			for (string s : table_vString(t.getTable("objects")))
			{
				objects.push_back(string_ponteiro<objeto_jogo>(s)->pegar_componente<transform_>());
			}
			mesh->transforms = objects;
			return 0;
		}
	}

	int set_ui_curson_location(lua_State *L)
	{
		vec2 cursor_pos = table_vec2(lua_totable(L, 1));
		ui_componente::cursor_position = cursor_pos;
		ui_componente::click = lua_toboolean(L, 2);
		return 0;
	}

	int set_ui_selection_id(lua_State *L)
	{
		ui_componente::selection_possition = lua_tonumber(L, 1);
		ui_componente::click = lua_toboolean(L, 2);
		return 0;
	}

	int get_set_ui_component(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<ui_componente> ui = obj->pegar_componente<ui_componente>();
			ret.setFloat("layer", ui->render_layer + 1);
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			ret.setString("text", converter.to_bytes(ui->text));
			ret.setFloat("ui_type", ui->ui_type);
			ret.setFloat("selection_id", ui->selection_id);
			ret.setString("id", ui->id);
			ret.setString("data", ui->data);
			ret.setString("state", ui->state);
			ret.setFloat("text_location_x", ui->text_location_x);
			ret.setFloat("text_location_y", ui->text_location_y);
			ret.setTable("position", vec2_table(ui->position));
			ret.setTable("scale", vec2_table(ui->scale));
			ret.setTable("normal_style", advanced_ui_style_table(ui->normal_style));
			ret.setTable("hover_style", advanced_ui_style_table(ui->hover_style));
			ret.setTable("click_style", advanced_ui_style_table(ui->click_style));

			lua_pushtable(L, ret);
			return 1;
		}
		else
		{

			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<ui_componente> ui = obj->pegar_componente<ui_componente>();

			ui->render_layer = t.getFloat("layer") - 1;
			ui->set_text_by_string(t.getString("text"));
			ui->state = t.getString("text");
			ui->ui_type = t.getFloat("ui_type");
			ui->selection_id = t.getFloat("selection_id");
			ui->id = t.getString("id");
			ui->data = t.getString("data");
			ui->text_location_x = t.getFloat("text_location_x");
			ui->text_location_y = t.getFloat("text_location_y");
			ui->position = table_vec2(t.getTable("position"));
			ui->scale = table_vec2(t.getTable("scale"));
			ui->normal_style = table_advanced_ui_style(t.getTable("normal_style"));
			ui->hover_style = table_advanced_ui_style(t.getTable("hover_style"));
			ui->click_style = table_advanced_ui_style(t.getTable("click_style"));

			return 0;
		}
	}

	int set_ui_component_function(lua_State *L)
	{
		objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		shared_ptr<ui_componente> ui = obj->pegar_componente<ui_componente>();
		if (string(lua_tostring(L, 2)) != "")
		{
			ui->lua_function = {L, lua_tostring(L, 2)};
		}
		else
		{
			ui->lua_function = {NULL, ""};
		}
		return 0;
	}

	int get_set_light(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<fonte_luz> ui = obj->pegar_componente<fonte_luz>();

			lua_pushtable(L, ret);
			return 1;
		}
		else
		{

			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<fonte_luz> ui = obj->pegar_componente<fonte_luz>();

			return 0;
		}
	}

	int raycast_2D(lua_State *L)
	{
		Table ret;
		colis_info ci;
		bool rca = box_2D::ray_cast(table_vec2(lua_totable(L, 1)), table_vec2(lua_totable(L, 2)), ci);
		ret = colis_info_table(ci);
		lua_pushboolean(L, rca);
		lua_pushtable(L, ret);
		return 2;
	}

	// camerareturn

	int get_set_camera(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<camera> cam = obj->pegar_componente<camera>();
			ret.setFloat("orthographc", cam->ortografica);
			ret.setTable("size", vec2_table(cam->tamanho));
			ret.setFloat("zoom", cam->zoom);
			ret.setTable("resolution", vec2_table(cam->res));
			ret.setFloat("fcp", cam->fcp);
			ret.setFloat("ncp", cam->ncp);
			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<camera> cam = obj->pegar_componente<camera>();
			if (t.getFloat("orthographc"))
			{
				vec2 size = table_vec2(t.getTable("size"));
				cam->configurar_camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), size.x, size.y, t.getFloat("ncp"), t.getFloat("fcp"));
			}
			else
			{
				vec2 res = table_vec2(t.getTable("resolution"));
				cam->configurar_camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), t.getFloat("zoom"), res.x, res.y, t.getFloat("ncp"), t.getFloat("fcp"));
			}
			return 0;
		}
	}

	int get_set_physic_3D(lua_State *L)
	{

		if (lua_tonumber(L, 1) == get_lua)
		{

			Table ret;
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();

			Table _mesh;
			if (bu->collision_mesh != NULL)
			{
				_mesh.setString("file", bu->collision_mesh->arquivo_origem);
				_mesh.setString("name", bu->collision_mesh->nome);
				ret.setTable("collision_mesh", _mesh);
			}
			else
			{
				_mesh.setString("file", "");
				_mesh.setString("name", "");
				ret.setTable("collision_mesh", _mesh);
			}

			ret.setTable("collision_layer", info_camada_table(bu->layer));
			ret.setTable("scale", vec3_table(bu->escala));
			ret.setFloat("boady_dynamic", bu->dinamica);
			ret.setFloat("collision_shape", bu->forma);

			ret.setFloat("rotate_x", bu->rotacionarX);
			ret.setFloat("rotate_y", bu->rotacionarY);
			ret.setFloat("rotate_z", bu->rotacionarZ);

			ret.setFloat("elasticity", bu->elasticidade);

			ret.setFloat("triger", bu->gatilho);
			ret.setFloat("friction", bu->atrito);
			ret.setFloat("density", bu->densidade);
			ret.setFloat("gravity_scale", bu->gravity_force);

			ret.setFloat("get_collision_info", bu->get_collision_info);

			vector<Table> colis_infos;
			set<string> set_objs_touching;

			colis_infos.resize(bu->colis_infos.size());
			for (size_t i = 0; i < bu->colis_infos.size(); i++)
			{
				colis_infos[i] = colis_info_table(bu->colis_infos[i]);
				set_objs_touching.insert(ponteiro_string(bu->colis_infos[i].cos_obj));
			}
			vector<string> objs_touching(set_objs_touching.begin(), set_objs_touching.end());

			ret.setTable("colis_infos", vTable_table(colis_infos));
			ret.setTable("objs_touching", vString_table(objs_touching));

			lua_pushtable(L, ret);
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			objeto_jogo *obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<bullet> bu = obj->pegar_componente<bullet>();

			Table mesh_info = t.getTable("collision_mesh");
			if (mesh_info.getString("file").compare("") && mesh_info.getString("name").compare(""))
			{
				bu->collision_mesh = ManuseioDados::carregar_malha(mesh_info.getString("file"), mesh_info.getString("name"));
			}

			bu->layer = table_info_camada(t.getTable("collision_layer"));
			bu->escala = table_vec3(t.getTable("scale"));
			bu->dinamica = t.getFloat("boady_dynamic");
			bu->forma = (char)t.getFloat("collision_shape");

			bu->rotacionarX = t.getFloat("rotate_x") > 0;
			bu->rotacionarY = t.getFloat("rotate_y") > 0;
			bu->rotacionarZ = t.getFloat("rotate_z") > 0;

			bu->elasticidade = t.getFloat("elasticity");

			bu->gatilho = t.getFloat("triger");
			bu->atrito = t.getFloat("friction");
			bu->densidade = t.getFloat("density");
			bu->gravity_force = t.getFloat("gravity_scale");
			bu->get_collision_info = t.getFloat("get_collision_info");
			bu->aplay();
			return 0;
		}
	}

	int remove_navmesh(lua_State *L)
	{
		navmesh::remove_navmesh(lua_tostring(L, 1));
		return 0;
	}

	int remove_all_navmesh(lua_State *L)
	{
		navmesh::remove_all_navmesh();
		return 0;
	}

	int create_navmesh(lua_State *L)
	{
		Table mesh_info = lua_totable(L, 4);
		navmesh::create_navmesh(table_vec3(lua_totable(L, 1)), graus_quat(table_vec3(lua_totable(L, 2))), table_vec3(lua_totable(L, 3)), ManuseioDados::carregar_malha(mesh_info.getString("file"), mesh_info.getString("name")), lua_tostring(L, 5));
		return 0;
	}

	int generate_navmesh_path(lua_State *L)
	{

		vector<Table> ret;
		vector<vec3> path = navmesh::generate_path(table_vec3(lua_totable(L, 1)), table_vec3(lua_totable(L, 2)), lua_tostring(L, 3));
		for (size_t i = 0; i < path.size(); i++)
		{
			ret.push_back(vec3_table(path[i]));
		}
		lua_pushtable(L, vTable_table(ret));
		return 1;
	}

	int generate_navmesh_short_path(lua_State *L)
	{

		vector<Table> ret;
		vector<vec3> path = navmesh::generate_path(table_vec3(lua_totable(L, 1)), table_vec3(lua_totable(L, 2)), lua_tostring(L, 3),2);
		for (size_t i = 0; i < path.size(); i++)
		{
			ret.push_back(vec3_table(path[i]));
		}
		lua_pushtable(L, vTable_table(ret));
		return 1;
	}

	int show_navmesh(lua_State *L)
	{
		navmesh::show(lua_toboolean(L, 1));
		return 0;
	}

	int look_to(lua_State *L)
	{
		vec3 ret;

		vec3 pos_a = table_vec3(lua_totable(L, 1));
		vec3 pos_b = table_vec3(lua_totable(L, 2));

		bool rot_x = lua_toboolean(L, 3);
		bool rot_y = lua_toboolean(L, 4);
		bool rot_z = lua_toboolean(L, 5);

		glm::vec3 direction = glm::normalize(pos_b - pos_a);

		// Criar uma orientação 'olhando para' usando LookAt, e então obter os ângulos de Euler
		glm::mat4 lookAtMatrix = glm::lookAt(pos_a, pos_b, glm::vec3(0, 0, -1));
		glm::quat quaternionRotation = glm::quat_cast(lookAtMatrix);
		glm::vec3 eulerAngles = glm::eulerAngles(quaternionRotation);

		// Converter de radianos para graus
		eulerAngles = glm::degrees(eulerAngles);

		// Se não precisar de rotação em algum eixo, defina esse eixo para 0
		if (!rot_x)
		{
			eulerAngles.x = 0;
		}
		if (!rot_y)
		{
			eulerAngles.y = 0;
		}
		if (!rot_z)
		{
			eulerAngles.z = 0;
		}

		ret = eulerAngles;

		lua_pushtable(L, vec3_table(ret));
		return 1;
	}

	int walk_along_the_path(lua_State *L)
	{
		Table ret;

		vector<Table> path_tables = table_vTable(lua_totable(L, 1));
		vector<glm::vec3> path;
		for (Table t : path_tables)
		{
			path.push_back(table_vec3(t));
		}
		if (!path.size())
		{
			return 0;
		}
		float current_progression = std::max(0.001, lua_tonumber(L, 2));
		float speed_or_walk_distance = lua_tonumber(L, 3);

		glm::vec3 target;
		glm::vec3 target_movement;
		glm::vec3 rotation;
		float progression;

		int current_index = static_cast<int>(current_progression);
		float lerp_factor = current_progression - current_index;

		if (current_index < 0 || current_index >= path.size() - 1)
		{
			// Não há próximo ponto
			return 0; // Retornar nil
		}

		// Interpolação linear para encontrar a posição atual e a próxima
		glm::vec3 current_position = path[current_index];
		glm::vec3 next_position = path[current_index + 1];
		glm::vec3 interpolated_position = current_position + lerp_factor * (next_position - current_position);

		// Calcular a distância até o próximo ponto
		float distance_to_next_point = glm::distance(current_position, next_position);

		// Atualizar progression
		progression = current_progression + speed_or_walk_distance / distance_to_next_point;
		if (progression >= path.size() - 1)
		{
			// Chegou ao fim do caminho
			return 0; // Retornar nil
		}

		// Calcular target e target_movement
		target = interpolated_position;
		target_movement = glm::normalize(next_position - current_position) * speed_or_walk_distance;

		bool rotate = true;

		if (glm::length(target_movement) > 0)
		{

			glm::vec3 position = current_position;
			glm::vec3 up = (vec3(0, 1, 0));

			glm::vec3 direction = glm::normalize(target - position);

			// Calcula os ângulos de pitch e yaw
			float pitch = glm::degrees(asin(-direction.y));
			float yaw = glm::degrees(atan2(direction.x, direction.z));

			// Constrói o vetor em graus
			glm::vec3 euler_angles = glm::vec3(pitch, yaw, 0.0f);

			euler_angles.z = up.z;

			// Os ângulos de Euler estão em radianos, converta-os para graus se necessário
			rotation = euler_angles;
		}
		else
		{
			rotate = false;
		}

		ret.setTable("position", vec3_table(target));
		ret.setTable("direction_movement", vec3_table(target_movement));
		if (rotate)
		{
			ret.setTable("rotation", vec3_table(rotation));
		}
		ret.setFloat("progression", progression);

		lua_pushtable(L, ret);
		return 1;
	}

	int get_objects_coliding(lua_State *L)
	{

		objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		shared_ptr<bullet> bu = obj->pegar_componente<bullet>();

		if (b2d != NULL)
		{

			set<string> objs_touching;
			for (shared_ptr<objeto_jogo> obj : b2d->objs_touching)
			{
				objs_touching.insert(ponteiro_string(obj.get()));
			}
			vector<string> vs(objs_touching.begin(), objs_touching.end());
			lua_pushtable(L, vString_table(vs));

			return 1;
		}
		else if (bu != NULL)
		{

			set<string> objs_touching;
			for (objeto_jogo *obj : bu_collisions_no_per_object[obj])
			{
				objs_touching.insert(ponteiro_string(obj));
			}

			vector<string> vs(objs_touching.begin(), objs_touching.end());

			lua_pushtable(L, vString_table(vs));

			return 1;
		}

		return 0;
	}

	int get_collision_infos(lua_State *L)
	{

		objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		shared_ptr<bullet> bu = obj->pegar_componente<bullet>();

		if (b2d != NULL)
		{
			vector<Table> colis_infos;
			for (colis_info ci : b2d->colis_infos)
			{
				colis_infos.push_back(colis_info_table(ci));
			}
			lua_pushtable(L, vTable_table(colis_infos));

			return 1;
		}
		else if (bu != NULL)
		{
			vector<Table> colis_infos;
			for (colis_info ci : bu->colis_infos)
			{
				colis_infos.push_back(colis_info_table(ci));
			}
			lua_pushtable(L, vTable_table(colis_infos));

			return 1;
		}

		return 0;
	}

	int raycast_3D(lua_State *L)
	{
		colis_info ci;
		bool rca = raycast_dir_bullet_3D(table_vec3(lua_totable(L, 1)), table_vec3(lua_totable(L, 2)), ci);
		lua_pushboolean(L, rca);
		lua_pushtable(L, colis_info_table(ci));
		return 2;
	}

	int global_data_get_var(lua_State *L)
	{
		string var_name = lua_tostring(L, 1);
		lua_getglobal(lua_global_data, var_name.c_str());
		int lua_type_id = lua_type(lua_global_data, -1);

		if (lua_type_id == LUA_TNUMBER)
		{
			lua_pushnumber(L, lua_tonumber(lua_global_data, -1));
		}
		else if (lua_type_id == LUA_TSTRING)
		{
			lua_pushstring(L, lua_tostring(lua_global_data, -1));
		}
		else if (lua_type_id == LUA_TBOOLEAN)
		{
			lua_pushboolean(L, lua_toboolean(lua_global_data, -1));
		}
		else if (lua_type_id == LUA_TTABLE)
		{
			lua_pushtable(L, lua_totable(lua_global_data, -2));
		}
		else if (lua_type_id == LUA_TNIL)
		{
			lua_pop(lua_global_data, 1);
			return 0;
		}
		lua_pop(lua_global_data, 1);
		return 1;
	}

	int global_data_set_var(lua_State *L)
	{

		string var_name = lua_tostring(L, 1);
		int lua_type_id = lua_type(L, 2);

		if (lua_type_id == LUA_TNUMBER)
		{
			lua_pushnumber(lua_global_data, lua_tonumber(L, 2));
			lua_setglobal(lua_global_data, var_name.c_str());
		}
		else if (lua_type_id == LUA_TSTRING)
		{
			lua_pushstring(lua_global_data, lua_tostring(L, 2));
			lua_setglobal(lua_global_data, var_name.c_str());
		}
		else if (lua_type_id == LUA_TBOOLEAN)
		{
			lua_pushboolean(lua_global_data, lua_toboolean(L, 2));
			lua_setglobal(lua_global_data, var_name.c_str());
		}
		else if (lua_type_id == LUA_TTABLE)
		{
			lua_pushtable(lua_global_data, lua_totable(L, 2));
			lua_setglobal(lua_global_data, var_name.c_str());
		}
		else if (lua_type_id == LUA_TNIL)
		{
			lua_pushnil(lua_global_data);
			lua_setglobal(lua_global_data, var_name.c_str());
		}

		// lua_gc(lua_global_data, LUA_GCSTEP, 0);

		return 0;
	}

	int get_scene_3D(lua_State *L)
	{
		string path = lua_tostring(L, 1);

		lua_pushtable(L, scene_3D_table_with_cache(ManuseioDados::carregar_modelo_3D(path)));

		return 1;
	}

	shared_ptr<objeto_jogo> display_lua_cubes = NULL;

	vec4 lua_cube_color = vec4(0, 0, 0, 1);

	int print_cube(lua_State *L)
	{

		navmesh_print_cube_in_space(table_vec3(lua_totable(L, 1)));

		/*
		if (!display_lua_cubes)
		{
			display_lua_cubes = novo_objeto_jogo();
		}

		shared_ptr<objeto_jogo> display_lua_cube = novo_objeto_jogo();
		display_lua_cube->adicionar_componente<transform_>();
		display_lua_cube->pegar_componente<transform_>()->pos = table_vec3(lua_totable(L, 1));
		display_lua_cube->pegar_componente<transform_>()->esca = vec3(1, 1, 1);

		display_lua_cube->adicionar_componente<render_malha>();
		shared_ptr<render_malha> rm = display_lua_cube->pegar_componente<render_malha>();

		rm->usar_oclusao = false;
		rm->camada = 4;

		Material mat;
		mat.shad = "resources/Shaders/mesh";

		if (lua_cube_color.x < 1)
		{
			lua_cube_color.x += 0.5;
		}
		else if (lua_cube_color.y < 1)
		{
			lua_cube_color.y += 0.5;
		}
		else if (lua_cube_color.z < 1)
		{
			lua_cube_color.z += 0.5;
		}
		else
		{
			lua_cube_color = vec4(0, 0, 0, 1);
		}

		mat.cor = lua_cube_color;
		mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");

		rm->malhas = {ManuseioDados::importar_obj("resources/3D Models/oclusion_box.obj")->malhas["Cube"]};

		rm->mats = {mat};
		cena_objetos_selecionados->adicionar_objeto(display_lua_cubes, display_lua_cube);
		*/

		return 0;
	}

	int clean_cube(lua_State *L)
	{
		if (display_lua_cubes)
		{
			cena_objetos_selecionados->remover_objeto(display_lua_cubes);
			display_lua_cubes = NULL;
		}
		return 0;
	}

	int set_keyframe(lua_State *L)
	{

		int args_no = lua_gettop(L);

		if (args_no == 5)
		{

			shared_ptr<cena_3D> scene = ManuseioDados::carregar_modelo_3D(lua_tostring(L, 1));

			vector<string> objects_ptrs_str = table_vString(lua_totable(L, 2));
			vector<objeto_jogo *> objects_ptrs = {};
			for (int i = 0; i < objects_ptrs_str.size(); i++)
			{
				objects_ptrs.push_back(string_ponteiro<objeto_jogo>(objects_ptrs_str[i]));
			}

			bool mix = lua_toboolean(L, 3);

			string animation_name = lua_tostring(L, 4);
			animacao ani = scene->animacoes[animation_name];

			float animation_time = lua_tonumber(L, 5);
			if (animation_time > ani.duration)
			{
				animation_time = ani.duration;
			}
			else if (animation_time < 0)
			{
				animation_time = 0;
			}

			float animation_frame = 0;
			if (animation_time > ani.duration)
			{
				animation_frame = ani.keyFrames.size();
			}
			else
			{
				animation_frame = (animation_time * ani.keyFrames.size()) / ani.duration + 1;
			}

			float animation_frame_rest = animation_frame - (int)animation_frame;

			if (animation_frame < 0)
			{
				animation_frame = 0;
				animation_frame_rest = 0;
			}
			else if (animation_frame > ani.keyFrames.size())
			{
				animation_frame = ani.keyFrames.size();
				animation_frame_rest = 0;
			}

			if (animation_frame_rest > 0 && animation_frame < ani.keyFrames.size() - 1 && mix)
			{
				vector<key_frame> kfs = mix_keyframes(ani.keyFrames[(int)animation_frame - 1], ani.keyFrames[((int)animation_frame)], animation_frame_rest);
				apply_key_frame_transform(kfs, objects_ptrs, scene->offset_matrices);
			}
			else
			{
				apply_key_frame_transform(ani.keyFrames[(int)animation_frame - 1], objects_ptrs, scene->offset_matrices);
			}
		}
		else if (args_no == 6)
		{
		}

		return 0;
	}

	map<string, map<string, lua_function>> funcoes_ponte_map_secoes = {
		pair<string, map<string, lua_function>>("file_system", {
																   pair<string, lua_function>("get_config_folder_path", funcoes_ponte::get_config_folder_path),
																   pair<string, lua_function>("create_directory", funcoes_ponte::create_directory),
															   }),
		pair<string, map<string, lua_function>>("game_object", {
																   pair<string, lua_function>("create_object", funcoes_ponte::create_object),
																   pair<string, lua_function>("get_object_with_name", funcoes_ponte::get_object_with_name),
																   pair<string, lua_function>("remove_object", funcoes_ponte::remove_object),
																   pair<string, lua_function>("add_component", add_component),
																   pair<string, lua_function>("remove_component", remove_component),
																   pair<string, lua_function>("reset_components", funcoes_ponte::reset_components),
																   pair<string, lua_function>("have_component", have_component),
																   pair<string, lua_function>("get_set_object", funcoes_ponte::get_set_object),
															   }),
		pair<string, map<string, lua_function>>("input", {
															 pair<string, lua_function>("set_cursor_position", funcoes_ponte::set_cursor_position),
															 pair<string, lua_function>("set_ui_selection_id", funcoes_ponte::set_ui_selection_id),
															 pair<string, lua_function>("set_keyboard_text_input", funcoes_ponte::set_keyboard_text_input),
															 pair<string, lua_function>("get_keyboard_text_input", funcoes_ponte::get_keyboard_text_input),
															 pair<string, lua_function>("get_input", funcoes_ponte::get_input),
														 }),
		pair<string, map<string, lua_function>>("time", {
															pair<string, lua_function>("get_time", funcoes_ponte::get_time),
															pair<string, lua_function>("set_time_scale", funcoes_ponte::set_time_scale),
														}),
		pair<string, map<string, lua_function>>("memory", {
															  pair<string, lua_function>("clear_memory", funcoes_ponte::clear_memory),
															  pair<string, lua_function>("is_loaded", funcoes_ponte::is_loaded),
															  pair<string, lua_function>("memory_usage_info", funcoes_ponte::memory_usage_info),
															  pair<string, lua_function>("make_banchmark", make_banchmark),

														  }),
		pair<string, map<string, lua_function>>("gravity", {
															   pair<string, lua_function>("to_move", funcoes_ponte::to_move),
															   pair<string, lua_function>("get_gravity", funcoes_ponte::get_gravity),
															   pair<string, lua_function>("set_gravity", funcoes_ponte::set_gravity),
														   }),
		pair<string, map<string, lua_function>>("transform", {
																 pair<string, lua_function>("get_set_transform", funcoes_ponte::get_set_transform),

																 pair<string, lua_function>("move_transform", funcoes_ponte::move_transform),
																 pair<string, lua_function>("rotate_transform", funcoes_ponte::rotate_transform),
																 pair<string, lua_function>("transform_look_at", funcoes_ponte::transform_look_at),
																 

																 pair<string, lua_function>("change_transfotm_position", funcoes_ponte::change_transfotm_position),
																 pair<string, lua_function>("change_transfotm_rotation", funcoes_ponte::change_transfotm_rotation),
																 pair<string, lua_function>("change_transfotm_scale", funcoes_ponte::change_transfotm_scale),

																 pair<string, lua_function>("get_local_direction_transform", funcoes_ponte::get_local_direction_transform),
																 pair<string, lua_function>("get_global_position_transform", funcoes_ponte::get_global_position_transform),
																 pair<string, lua_function>("get_translation_position_transform", funcoes_ponte::get_translation_position_transform),
															 }),
		pair<string, map<string, lua_function>>("window", {
															  pair<string, lua_function>("get_set_window", funcoes_ponte::get_set_window),
															  pair<string, lua_function>("close", funcoes_ponte::close),
															  pair<string, lua_function>("set_cursor_image", funcoes_ponte::set_cursor_image),
															  pair<string, lua_function>("enable_cursor", funcoes_ponte::enable_cursor),
														  }),
		pair<string, map<string, lua_function>>("render", {
															  pair<string, lua_function>("get_set_render_layer_instruction", funcoes_ponte::get_set_render_layer_instruction),
															  pair<string, lua_function>("get_set_sprite_render", funcoes_ponte::get_set_sprite_render),
															  pair<string, lua_function>("get_set_render_tilemap", funcoes_ponte::get_set_render_tilemap),
															  pair<string, lua_function>("get_set_render_text", funcoes_ponte::get_set_render_text),
															  pair<string, lua_function>("get_text_size", funcoes_ponte::get_text_size),
															  pair<string, lua_function>("get_set_render_shader", funcoes_ponte::get_set_render_shader),
															  pair<string, lua_function>("get_set_camera", funcoes_ponte::get_set_camera),
															  pair<string, lua_function>("get_set_render_mesh", funcoes_ponte::get_set_render_mesh),
															  pair<string, lua_function>("get_set_render_poly_mesh", funcoes_ponte::get_set_render_poly_mesh),
															  pair<string, lua_function>("get_scene_3D", funcoes_ponte::get_scene_3D),
															  pair<string, lua_function>("set_keyframe", funcoes_ponte::set_keyframe),
															  pair<string, lua_function>("get_set_post_processing", funcoes_ponte::get_set_post_processing),
															  pair<string, lua_function>("set_ui_curson_location", funcoes_ponte::set_ui_curson_location),
															  pair<string, lua_function>("get_set_ui_component", funcoes_ponte::get_set_ui_component),
															  pair<string, lua_function>("set_ui_component_function", funcoes_ponte::set_ui_component_function),

														  }),
		pair<string, map<string, lua_function>>("physics", {
															   pair<string, lua_function>("get_set_physic_2D", funcoes_ponte::get_set_physic_2D),
															   pair<string, lua_function>("add_force", funcoes_ponte::add_force),
															   pair<string, lua_function>("add_impulse", funcoes_ponte::add_impulse),
															   pair<string, lua_function>("set_linear_velocity", funcoes_ponte::set_linear_velocity),
															   pair<string, lua_function>("set_angular_velocity", funcoes_ponte::set_angular_velocity),
															   pair<string, lua_function>("add_rotative_force", funcoes_ponte::add_rotative_force),
															   pair<string, lua_function>("add_rotative_impulse", funcoes_ponte::add_rotative_impulse),
															   pair<string, lua_function>("raycast_2D", funcoes_ponte::raycast_2D),
															   pair<string, lua_function>("get_set_physic_3D", funcoes_ponte::get_set_physic_3D),
															   pair<string, lua_function>("get_objects_coliding", funcoes_ponte::get_objects_coliding),
															   pair<string, lua_function>("get_collision_infos", funcoes_ponte::get_collision_infos),
															   pair<string, lua_function>("raycast_3D", funcoes_ponte::raycast_3D),

														   }),
		pair<string, map<string, lua_function>>("navmesh", {
															   pair<string, lua_function>("remove_navmesh", remove_navmesh),
															   pair<string, lua_function>("remove_all_navmesh", remove_all_navmesh),
															   pair<string, lua_function>("create_navmesh", create_navmesh),
															   pair<string, lua_function>("generate_navmesh_path", generate_navmesh_path),
															   pair<string, lua_function>("generate_navmesh_short_path", generate_navmesh_short_path),
															   pair<string, lua_function>("show_navmesh", show_navmesh),
															   pair<string, lua_function>("look_to", look_to),
															   pair<string, lua_function>("walk_along_the_path", walk_along_the_path),

														   }),
		pair<string, map<string, lua_function>>("audio", {
															 pair<string, lua_function>("get_set_audio", get_set_audio),
															 pair<string, lua_function>("get_set_global_volume", get_set_global_volume),
															 pair<string, lua_function>("set_lisener_object", set_lisener_object),
														 }),
		pair<string, map<string, lua_function>>("script", {
															  pair<string, lua_function>("get_script_size", get_script_size),
															  pair<string, lua_function>("set_script_var", set_script_var),
															  pair<string, lua_function>("get_lua_component", get_lua_component),
															  pair<string, lua_function>("have_script", have_script),
															  pair<string, lua_function>("add_script_lua", add_script_lua),
															  pair<string, lua_function>("remove_script", remove_script),
															  pair<string, lua_function>("get_lua_var", get_lua_var),
															  pair<string, lua_function>("set_lua_var", set_lua_var),
															  pair<string, lua_function>("call_lua_function", call_lua_function),
														  }),
		pair<string, map<string, lua_function>>("global_data", {
																   pair<string, lua_function>("global_data_get_var", global_data_get_var),
																   pair<string, lua_function>("global_data_set_var", global_data_set_var),
															   }),
		pair<string, map<string, lua_function>>("3D_sceane", {
																 pair<string, lua_function>("get_scene_3D", get_scene_3D),
															 }),
		pair<string, map<string, lua_function>>("debug", {
															 pair<string, lua_function>("print_cube", print_cube),
															 pair<string, lua_function>("clean_cube", clean_cube),

														 }),
	};

};

int register_function_set(lua_State *L, string set_de_funcoes)
{

	if (set_de_funcoes == "all")
	{
		for (pair<string, map<string, lua_function>> p1 : funcoes_ponte::funcoes_ponte_map_secoes)
		{
			for (pair<string, lua_function> p2 : p1.second)
			{
				lua_register(L, p2.first.c_str(), p2.second);
				lua_register(L, ("c_" + p2.first).c_str(), p2.second);
			}
		}
	}
	else
	{
		for (pair<string, lua_function> p : funcoes_ponte::funcoes_ponte_map_secoes[set_de_funcoes])
		{
			lua_register(L, p.first.c_str(), p.second);
			lua_register(L, ("c_" + p.first).c_str(), p.second);
		}
	}
	return 0;
}

int register_function_set(lua_State *L)
{
	register_function_set(L, lua_tostring(L, 1));

	return 0;
}

void load_base_lua_state(lua_State *L, string path)
{

	shared_ptr<string> compiledCode;
	auto load_script_thread = [](string path, shared_ptr<string> *ret)
	{ *ret = carregar_script_lua(path); };

	thread cct(load_script_thread, path, &compiledCode);

	luaL_openlibs(L);

	// configurar diretorio
	string lua_path = argumentos[1] + "/resources/Scripts/?.lua;" + argumentos[1] + "/resources/Scripts/engine_libs/?.lua";

	string link_libs_path = argumentos[1] + string("/resources/Scripts/link_libs/?.dll;") + argumentos[1] + string("/resources/Scripts/link_libs/?.so");

	lua_getglobal(L, "package");
	lua_pushstring(L, lua_path.c_str());
	lua_setfield(L, -2, "path");
	lua_pushstring(L, link_libs_path.c_str());
	lua_setfield(L, -2, "cpath");
	lua_pop(L, 1);

#ifdef USE_LUA_JIT
	luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
	lua_gc(L, LUA_GCSETPAUSE, 100);
	lua_gc(L, LUA_GCSETSTEPMUL, 400);
#else
	lua_gc(L, LUA_GCSETSTEPMUL, 1000);
#endif

	lua_newtable(L);
	for (int i = 0; i < argumentos.size(); i++)
	{
		lua_pushinteger(L, i);
		lua_pushstring(L, argumentos[i].c_str());
		lua_settable(L, -3);
	}
	lua_setglobal(L, "args");
	lua_register(L, "register_function_set", register_function_set);

	// shared_ptr<string> compiledCode = carregar_script_lua(path);
	cct.join();

	int loadResult = luaL_loadbuffer(L, compiledCode->c_str(), compiledCode->size(), "compiled_script");

	if (loadResult == LUA_OK)
	{
		// Execute o código Lua carregado
		int execResult = lua_pcall(L, 0, LUA_MULTRET, 0);

		if (execResult != LUA_OK)
		{
			std::cerr << "Error executing Lua script: " << lua_tostring(L, -1) << std::endl;
		}
	}
	else
	{
		escrever("LUA NOT OK");
		std::cerr << "Error loading compiled Lua script: " << lua_tostring(L, -1) << std::endl;
	}
}

namespace funcoes_lua
{

	void adicionar_parte_funcoes_ponte_estado_lua(lua_State *L, vector<pair<string, lua_function>> funcoes, int inicio, int tamanho)
	{
		for (int i = inicio; i < tamanho; i++)
		{
			lua_register(L, funcoes[i].first.c_str(), funcoes[i].second);
		}
	}

	map<string, string> scripts_lua;
	void limpar_scripts_lua()
	{
		scripts_lua.clear();
	}

	lua_State *carregar_luaState(string s)
	{
		// CompileLuaScriptToString
		// LoadCompiledLuaScriptFromString

		// criar
		lua_State *ret = luaL_newstate();

		load_base_lua_state(ret, s);

		return ret;
	}

};

vector<thread> lua_threads_to_clean;

void clean_lua_threads()
{

	for (std::thread &thread : lua_threads_to_clean)
	{
		thread.join();
	}
	lua_threads_to_clean.clear();
}

class componente_lua : public componente
{
	bool iniciado = false;
	unordered_map<string, shared_ptr<string>> scripts_lua_string;
	unordered_map<string, bool> scripts_lua_iniciados;

public:
	unordered_map<string, lua_State *> estados_lua;
	set<lua_State *> to_benchmark;

	vector<string> pegar_lista_scripts()
	{
		vector<string> ret = {};
		for (pair<string, shared_ptr<string>> p : scripts_lua_string)
		{
			ret.push_back(p.first);
		}
		return ret;
	}

	unordered_map<string, lua_State *> pegar_estados_lua()
	{
		return estados_lua;
	}

	void deletar_estado_lua(lua_State *L)
	{
		lua_close(L);
	}

	void remover_script(string s)
	{
		if (estados_lua.find(s) != estados_lua.end())
		{
			deletar_estado_lua(estados_lua[s]);
			estados_lua.erase(s);
			scripts_lua_string.erase(s);
			scripts_lua_iniciados.erase(s);
		}
	}
	void remover_scripts(vector<string> s)
	{
		for (string S : s)
		{
			remover_script(S);
		}
	}
	void limpar_scripts()
	{
		vector<string> scripts;
		for (pair<string, lua_State *> p : estados_lua)
		{
			scripts.push_back(p.first);
		}
		remover_scripts(scripts);
	}

	void adicionar_script(string s)
	{

		if (estados_lua.find(s) != estados_lua.end())
		{
			lua_getglobal(estados_lua[s], "END");
			lua_call(estados_lua[s], 0, 0);
			remover_script(s);
		}

		lua_State *L = funcoes_lua::carregar_luaState(s);
		estados_lua.insert(pair<string, lua_State *>(s, L));
		scripts_lua_string.insert(pair<string, shared_ptr<string>>(s, carregar_script_lua(s)));
		scripts_lua_iniciados.insert(pair<string, bool>(s, false));

		lua_pushstring(L, ponteiro_string(esse_objeto.get()).c_str());
		lua_setglobal(L, "this_object_ptr");
	}
	void adicionar_scripts(vector<string> s)
	{
		for (string S : s)
		{
			adicionar_script(S);
		}
	}

	void iniciar()
	{

		iniciado = true;

		vector<pair<string, lua_State *>> pairs(estados_lua.begin(), estados_lua.end());
		reverse(pairs.begin(), pairs.end());

		for (pair<string, lua_State *> p : pairs)
		{

			// pair<string, lua_State *> p = pairs[i];

			if (!scripts_lua_iniciados[p.first])
			{

				// esse objetoget_tile_set_tile
				lua_pushstring(p.second, ponteiro_string(esse_objeto.get()).c_str());
				lua_setglobal(p.second, "this_object_ptr");

				lua_getglobal(p.second, "START");
				lua_call(p.second, 0, 0);
				scripts_lua_iniciados[p.first] = true;
			}
		}
	}
	void atualisar()
	{

		// Benchmark_Timer bt("update_lua_scripts");

		// iniciar();

		vector<pair<string, lua_State *>> pairs(estados_lua.begin(), estados_lua.end());
		reverse(pairs.begin(), pairs.end());

		// for (int i = 0 ; i < pairs.size() ; i++)
		// for (int i = pairs.size() - 1; i >= 0; i--)

		bool shold_start = false;
		for (pair<string, lua_State *> p : pairs)
		{

			// pair<string, lua_State *> p = pairs[i];

			if (scripts_lua_iniciados[p.first])
			{

				bool benchmark = false;

				for (lua_State *ls : to_benchmark)
				{
					if (ls == p.second)
					{
						benchmark = true;
						break;
					}
				}

				if (benchmark)
				{

					Benchmark_Timer bt(p.first);
					lua_State *L = p.second;
					lua_getglobal(p.second, "UPDATE");
					lua_call(L, 0, 0);
				}
				else
				{
					lua_State *L = p.second;
					lua_getglobal(p.second, "UPDATE");
					lua_call(L, 0, 0);
				}
			}
			else
			{
				shold_start = true;
			}
		}

		if (shold_start)
		{
			iniciar();
		}
	}

	void finalisar()
	{
		vector<pair<string, lua_State *>> pairs(estados_lua.begin(), estados_lua.end());
		reverse(pairs.begin(), pairs.end());

		for (pair<string, lua_State *> p : pairs)
		{

			lua_State *L = p.second;

			lua_getglobal(L, "END");
			lua_call(L, 0, 0);
		}
	}
	~componente_lua()
	{
		limpar_scripts();
	}

	// mudar
	void mudar_string(string script, string var, string valor)
	{
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_pushstring(estados_lua[script], valor.c_str());
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_int(string script, string var, int valor)
	{
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_pushinteger(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_numero(string script, string var, float valor)
	{
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_pushnumber(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_boleana(string script, string var, bool valor)
	{
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_pushboolean(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_tabela(string script, string var, Table valor)
	{
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_pushtable(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}

	// pegar
	string pegar_string(string script, string var)
	{
		string ret = "";
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tostring(estados_lua[script], -1);
		}
		return ret;
	}
	int pegar_int(string script, string var)
	{
		int ret = 0;
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tointeger(estados_lua[script], -1);
		}
		return ret;
	}
	float pegar_numero(string script, string var)
	{
		float ret = 0;
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tonumber(estados_lua[script], -1);
		}
		return ret;
	}
	bool pegar_boleana(string script, string var)
	{
		bool ret = false;
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_toboolean(estados_lua[script], -1);
		}
		return ret;
	}
	Table pegar_tabela(string script, string var)
	{
		Table ret;
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_totable(estados_lua[script], -2);
		}
		return ret;
	}

	// função
	void chamar_funcao(string script, string func)
	{
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_getglobal(estados_lua[script], func.c_str());
			lua_call(estados_lua[script], 0, 0);
		}
	}

	Table chamar_funcao_lua(string script, string func, Table arg)
	{
		Table ret;
		if (estados_lua.find(script) != estados_lua.end())
		{
			lua_getglobal(estados_lua[script], func.c_str());
			lua_pushtable(estados_lua[script], arg);
			// lua_call(estados_lua[script], 1, 1);
			if (lua_pcall(estados_lua[script], 1, LUA_MULTRET, 0) != 0)
			{
				return ret;
			}
			if (lua_gettop(estados_lua[script]) - 1 > 0)
			{
				ret = lua_totable(estados_lua[script], -2);
			}
		}
		return ret;
	}

	componente_lua()
	{
	}
	componente_lua(string s)
	{
		adicionar_script(s);
	}
	componente_lua(vector<string> s)
	{
		adicionar_scripts(s);
	}
};

map<string, void (*)(objeto_jogo *, bool)> add_remove_component_by_string = {
	pair<string, void (*)(objeto_jogo *, bool)>("transform", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<transform_>(transform_());}else{obj->remover_componente<transform_>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("camera", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<camera>(camera());}else{obj->remover_componente<camera>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_text", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<render_texto>(render_texto());}else{obj->remover_componente<render_texto>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_sprite", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<render_sprite>(render_sprite());}else{obj->remover_componente<render_sprite>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_shader", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<render_shader>(render_shader());}else{obj->remover_componente<render_shader>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("physics_2D", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<box_2D>(box_2D());}else{obj->remover_componente<box_2D>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("character_physics_2D", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<fisica_char_B2D>(fisica_char_B2D());}else{obj->remover_componente<fisica_char_B2D>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("physics_3D", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<bullet>(bullet());}else{obj->remover_componente<bullet>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("character_physics_3D", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<bullet_charter>(bullet_charter());}else{obj->remover_componente<bullet_charter>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("audio_source", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<audio_source>(audio_source());}else{obj->remover_componente<audio_source>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("lua_scripts", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<componente_lua>(componente_lua());}else{obj->remover_componente<componente_lua>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_tile_map", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<render_tilemap>(render_tilemap());}else{obj->remover_componente<render_tilemap>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_mesh", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<render_malha>(render_malha());}else{obj->remover_componente<render_malha>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("light", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<fonte_luz>(fonte_luz());}else{obj->remover_componente<fonte_luz>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_poly_mesh", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<poly_mesh>(poly_mesh());}else{obj->remover_componente<poly_mesh>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("ui_component", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<ui_componente>(ui_componente());}else{obj->remover_componente<ui_componente>();} }),

};

int add_component(lua_State *L)
{
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2)
	{
		add_remove_component_by_string[lua_tostring(L, 2)](obj, true);
	}

	return 0;
}

int remove_component(lua_State *L)
{
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2)
	{
		add_remove_component_by_string[lua_tostring(L, 2)](obj, false);
	}

	return 0;
}

int get_component_size(lua_State *L)
{
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	int ret = 0;
	if (argumentos == 1)
	{
		ret = obj->pegar_lista_componentes().size();
	}

	lua_pushnumber(L, ret);
	return 1;
}

map<string, bool (*)(objeto_jogo *)> have_component_by_string = {
	pair<string, bool (*)(objeto_jogo *)>("transform", [](objeto_jogo *obj)
										  { return obj->tem_componente<transform_>(); }),
	pair<string, bool (*)(objeto_jogo *)>("camera", [](objeto_jogo *obj)
										  { return obj->tem_componente<camera>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_text", [](objeto_jogo *obj)
										  { return obj->tem_componente<render_texto>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_sprite", [](objeto_jogo *obj)
										  { return obj->tem_componente<render_sprite>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_shader", [](objeto_jogo *obj)
										  { return obj->tem_componente<render_shader>(); }),
	pair<string, bool (*)(objeto_jogo *)>("physics_2D", [](objeto_jogo *obj)
										  { return obj->tem_componente<box_2D>(); }),
	pair<string, bool (*)(objeto_jogo *)>("character_physics_2D", [](objeto_jogo *obj)
										  { return obj->tem_componente<fisica_char_B2D>(); }),
	pair<string, bool (*)(objeto_jogo *)>("physics_3D", [](objeto_jogo *obj)
										  { return obj->tem_componente<bullet>(); }),
	pair<string, bool (*)(objeto_jogo *)>("character_physics_3D", [](objeto_jogo *obj)
										  { return obj->tem_componente<bullet_charter>(); }),
	pair<string, bool (*)(objeto_jogo *)>("audio_source", [](objeto_jogo *obj)
										  { return obj->tem_componente<audio_source>(); }),
	pair<string, bool (*)(objeto_jogo *)>("lua_scripts", [](objeto_jogo *obj)
										  { return obj->tem_componente<componente_lua>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_tile_map", [](objeto_jogo *obj)
										  { return obj->tem_componente<render_tilemap>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_mesh", [](objeto_jogo *obj)
										  { return obj->tem_componente<render_malha>(); }),
	pair<string, bool (*)(objeto_jogo *)>("light", [](objeto_jogo *obj)
										  { return obj->tem_componente<fonte_luz>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_poly_mesh", [](objeto_jogo *obj)
										  { return obj->tem_componente<poly_mesh>(); }),
	pair<string, bool (*)(objeto_jogo *)>("ui_component", [](objeto_jogo *obj)
										  { return obj->tem_componente<ui_componente>(); }),

};
int have_component(lua_State *L)
{
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	bool ret = false;
	if (argumentos == 2)
	{
		ret = have_component_by_string[lua_tostring(L, 2)](obj);
	}

	lua_pushboolean(L, ret);
	return 1;
}

// scripts

int get_script_size(lua_State *L)
{
	int output = 0;
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 1 && obj != NULL && obj->pegar_componente<componente_lua>() != NULL)
	{
		output = obj->pegar_componente<componente_lua>()->pegar_lista_scripts().size();
	}
	lua_pushnumber(L, output);
	return 1;
}

int set_script_var(lua_State *L)
{
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 5 && cl != NULL)
	{

		if (lua_tonumber(L, 4) == 0)
		{
			cl->mudar_string(lua_tostring(L, 2), lua_tostring(L, 3), lua_tostring(L, 5));
		}
		else if (lua_tonumber(L, 4) == 1)
		{
			cl->mudar_numero(lua_tostring(L, 2), lua_tostring(L, 3), lua_tonumber(L, 5));
		}
		else if (lua_tonumber(L, 4) == 2)
		{
			cl->mudar_boleana(lua_tostring(L, 2), lua_tostring(L, 3), lua_toboolean(L, 5));
		}
	}
	return 0;
}

// new script fonctions

int have_script(lua_State *L)
{
	bool output = false;
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL)
	{
		unordered_map<string, lua_State *> m = cl->pegar_estados_lua();
		if (m.find(lua_tostring(L, 2)) != m.end())
		{
			output = true;
		}
	}
	lua_pushboolean(L, output);
	return 1;
}

int add_script_lua(lua_State *L)
{
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL)
	{

		cl->adicionar_script(lua_tostring(L, 2));
	}
	return 0;
}

int remove_script(lua_State *L)
{
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL)
	{
		cl->remover_script(lua_tostring(L, 2));
	}
	return 0;
}

int get_lua_component(lua_State *L)
{
	Table ret;
	objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (cl != NULL)
	{
		ret.setTable("scripts", vString_table(cl->pegar_lista_scripts()));
	}
	lua_pushtable(L, ret);
	return 1;
}

int get_lua_var(lua_State *L)
{
	int argumentos = lua_gettop(L);

	objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	string script_name = lua_tostring(L, 2), var_name = lua_tostring(L, 3);

	lua_State *clL = cl->estados_lua[script_name];

	lua_getglobal(clL, var_name.c_str());
	int lua_type_id = lua_type(clL, -1);

	if (lua_type_id == LUA_TNUMBER)
	{
		lua_pushnumber(L, cl->pegar_numero(script_name, var_name));
	}
	else if (lua_type_id == LUA_TSTRING)
	{
		lua_pushstring(L, cl->pegar_string(script_name, var_name).c_str());
	}
	else if (lua_type_id == LUA_TBOOLEAN)
	{
		lua_pushboolean(L, cl->pegar_boleana(script_name, var_name));
	}
	else if (lua_type_id == LUA_TTABLE)
	{
		lua_pushtable(L, cl->pegar_tabela(script_name, var_name));
	}
	else if (lua_type_id == LUA_TNIL)
	{
		lua_pushnil(L);
	}
	return 1;
}

int set_lua_var(lua_State *L)
{
	objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	string script_name = lua_tostring(L, 2), var_name = lua_tostring(L, 3);
	lua_getglobal(cl->estados_lua[script_name], var_name.c_str());
	int lua_type_id = lua_type(L, 4);
	if (lua_type_id == LUA_TNUMBER)
	{
		cl->mudar_numero(script_name, var_name, lua_tonumber(L, 4));
	}
	else if (lua_type_id == LUA_TSTRING)
	{
		cl->mudar_string(script_name, var_name, lua_tostring(L, 4));
	}
	else if (lua_type_id == LUA_TBOOLEAN)
	{
		cl->mudar_numero(script_name, var_name, lua_tonumber(L, 4));
	}
	else if (lua_type_id == LUA_TTABLE)
	{
		cl->mudar_tabela(script_name, var_name, lua_totable(L, 4));
	}

	return 0;
}

int call_lua_function(lua_State *L)
{
	objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	string script_name = lua_tostring(L, 2), func_name = lua_tostring(L, 3);
	Table t = lua_totable(L, 4);
	Table ret = cl->chamar_funcao_lua(script_name, func_name, t);
	lua_pushtable(L, ret);
	return 1;
}

int make_banchmark(lua_State *L)
{
	objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	lua_State *L2 = cl->estados_lua[lua_tostring(L, 2)];
	cl->to_benchmark.insert(L2);
	return 0;
}
