#pragma once

#include "RecursosT.h"
#include "ManusearArquivos.h"
#include "LoopPrincipal.h"
#include "input.h"
#include "Tempo.h"
using namespace Tempo;
#include "scene.h"
#include "box_2d.h"
#include "sfml_audio.h"
#include "lua/lua.hpp"
#include "render_mesh.h"
#include "projetil.h"
#include "render_sprite.h"
#include "render_tilemap.h"
#include "bullet.h"
#include "light.h"

#include "args.h"
#include "game_object.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include "table.h"
#include "table_conversors.h"

#include "ui_mouse_cheker.h"

const int get_lua = 0;
const int set_lua = 1;

bool parallel_loading = false;

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
	print({"iniciando lua global data"});
	lua_global_data = luaL_newstate();
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

mapeamento_assets<string> mapeamento_scripts_lua;

shared_ptr<string> carregar_script_lua(string local)
{
	if (mapeamento_scripts_lua.pegar(local) == NULL)
	{
		mapeamento_scripts_lua.aplicar(local, ManuseioDados::Carregar_script(local));
	}
	return mapeamento_scripts_lua.pegar(local);
}

// aqui

int add_component(lua_State *L);
int remove_component(lua_State *L);
int get_component_size(lua_State *L);
int have_component(lua_State *L);

// scripts
int get_script_size(lua_State *L);
int get_script_name(lua_State *L);
int set_script_var(lua_State *L);

int get_lua_component(lua_State *L);
int have_script(lua_State *L);
int add_script_lua(lua_State *L);
int remove_script(lua_State *L);

int get_lua_var(lua_State *L);
int set_lua_var(lua_State *L);
int call_lua_function(lua_State *L);

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
		lua_pushnumber(L, Tempo::tempo);
		lua_pushnumber(L, Tempo::varTempRender);
		lua_pushnumber(L, Tempo::velocidadeTempo);
		return 3;
	}

	int set_time_scale(lua_State *L)
	{
		Tempo::velocidadeTempo = lua_tointeger(L, 1);
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
		buffers_som_sfml.limpar_lixo();
		ManuseioDados::mapeamento_fontes.limpar_lixo();
		ManuseioDados::mapeamento_imagems.limpar_lixo();
		ManuseioDados::mapeamento_tilesets.limpar_lixo();
		ManuseioDados::mapeamento_tile_map_infos.limpar_lixo();
		ManuseioDados::cenas_3D.limpar_lixo();
		mapeamento_scripts_lua.limpar_lixo();
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
			if (!ret && load && ManuseioDados::loading_requests_files.find(file_path) == ManuseioDados::loading_requests_files.end())
			{
				thread loader(ManuseioDados::carregar_Imagem, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 2)
		{
			ret = ManuseioDados::mapeamento_fontes.pegar(file_path) != NULL;
			if (!ret && load && ManuseioDados::loading_requests_files.find(file_path) == ManuseioDados::loading_requests_files.end())
			{
				thread loader(ManuseioDados::carregar_fonte, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 3)
		{
			ret = ManuseioDados::mapeamento_tilesets.pegar(file_path) != NULL;
			if (!ret && load && ManuseioDados::loading_requests_files.find(file_path) == ManuseioDados::loading_requests_files.end())
			{
				thread loader(ManuseioDados::carregar_tile_set, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 4)
		{
			ret = ManuseioDados::mapeamento_tile_map_infos.pegar(file_path) != NULL;
			if (!ret && load && ManuseioDados::loading_requests_files.find(file_path) == ManuseioDados::loading_requests_files.end())
			{
				thread loader(ManuseioDados::carregar_info_tile_map, file_path);
				loader.detach();
			}
		}
		else if (asset_type == 5)
		{
			ret = ManuseioDados::cenas_3D.pegar(file_path) != NULL;
			if (!ret && load && ManuseioDados::loading_requests_files.find(file_path) == ManuseioDados::loading_requests_files.end())
			{
				thread loader(ManuseioDados::carregar_modelo_3D, file_path);
				loader.join();
				// loader.detach();
			}
		}

		lua_pushboolean(L, ret);
		return 1;
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

				if (j.botoes.find(key) != j.botoes.end())
				{
					ret = j.botoes[key];
				}
				else if (j.eixos.find(key) != j.eixos.end())
				{
					ret = j.eixos[key];
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
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{
			tf->pos = v3;
		}
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		if (b2d != NULL)
		{
			b2d->mudar_pos(vec2(v3.x, v3.y));
		}
		shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
		if (bu != NULL)
		{
			bu->mudar_pos(v3);
		}
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
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL)
		{
			tf->mudar_angulo_graus(v3);
		}
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		if (b2d != NULL)
		{
			b2d->mudar_rot(v3.x);
		}
		shared_ptr<bullet> bu = obj->pegar_componente<bullet>();
		if (bu != NULL)
		{
			bu->mudar_rot(v3);
		}
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
			vector<string> objs_touching;
			for (shared_ptr<objeto_jogo> obj : b2d->objs_touching)
			{
				objs_touching.push_back(ponteiro_string(obj.get()));
			}
			ret.setTable("objs_touching", vString_table(objs_touching));
			ret.setTable("collision_layer", info_camada_table(b2d->camada));
			vector<Table> vertex;
			for (vec2 v2 : b2d->vertices)
			{
				vertex.push_back(vec2_table(v2));
			}
			ret.setTable("vertex", vTable_table(vertex));
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
			shared_ptr<sfml_audio> au = obj->pegar_componente<sfml_audio>();
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
			shared_ptr<sfml_audio> au = obj->pegar_componente<sfml_audio>();

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

	// post_processing
	int get_set_post_processing(lua_State *L)
	{
		if (lua_tonumber(L, 1) == get_lua)
		{
			Table ret;
			lua_pushtable(L, material_table(api_grafica->pos_processamento_info));
			return 1;
		}
		else
		{
			Table t = lua_totable(L, 2);
			api_grafica->pos_processamento_info = table_material(t);
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

	// audio
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

			ret.setFloat("rotate_X", bu->rotacionarX);
			ret.setFloat("rotate_Y", bu->rotacionarY);
			ret.setFloat("rotate_Z", bu->rotacionarZ);

			ret.setFloat("triger", bu->gatilho);
			ret.setFloat("friction", bu->atrito);
			ret.setFloat("density", bu->densidade);
			ret.setFloat("gravity_scale", bu->gravity_force);

			vector<string> objs_touching;
			for (objeto_jogo *obj : bu_collisions_no_per_object[obj])
			{
				objs_touching.push_back(ponteiro_string(obj));
			}
			ret.setTable("objs_touching", vString_table(stringRemoveDuplicates(objs_touching)));

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

			bu->rotacionarX = t.getFloat("rotate_X");
			bu->rotacionarY = t.getFloat("rotate_Y");
			bu->rotacionarZ = t.getFloat("rotate_Z");

			bu->gatilho = t.getFloat("triger");
			bu->atrito = t.getFloat("friction");
			bu->densidade = t.getFloat("density");
			bu->gravity_force = t.getFloat("gravity_scale");

			bu->aplay();
			return 0;
		}
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

		return 0;
	}

	int get_scene_3D(lua_State *L)
	{
		Table ret;
		string path = lua_tostring(L, 1);

		cena_3D scene = *ManuseioDados::carregar_modelo_3D(path).get();

		ret = scene_3D_table(scene);
		lua_pushtable(L, ret);
		return 1;
	}

	
	map<string, map<string, lua_function>> funcoes_ponte_map_secoes = {
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
														  		pair<string, lua_function>("get_scene_3D", get_scene_3D),
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
															   pair<string, lua_function>("raycast_3D", funcoes_ponte::raycast_3D),
														   }),
		pair<string, map<string, lua_function>>("audio", {
															 pair<string, lua_function>("get_set_audio", funcoes_ponte::get_set_audio),
															 pair<string, lua_function>("get_set_global_volume", funcoes_ponte::get_set_global_volume),
															 pair<string, lua_function>("set_lisener_object", funcoes_ponte::set_lisener_object),
														 }),
		pair<string, map<string, lua_function>>("script", {
															  pair<string, lua_function>("get_script_size", get_script_size),
															  pair<string, lua_function>("get_script_name", get_script_name),
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
	};

};

namespace funcoes_lua
{

	void adicionar_parte_funcoes_ponte_estado_lua(lua_State *L, vector<pair<string, lua_function>> funcoes, int inicio, int tamanho)
	{
		for (int i = inicio; i < tamanho; i++)
		{
			lua_register(L, funcoes[i].first.c_str(), funcoes[i].second);
		}
	}

	void registrar_funcoes_ponte_estado_lua(lua_State *L, string set_de_funcoes)
	{
		if (set_de_funcoes == "all")
		{
			for (pair<string, map<string, lua_function>> p1 : funcoes_ponte::funcoes_ponte_map_secoes)
			{
				for (pair<string, lua_function> p : p1.second)
				{
					lua_register(L, p.first.c_str(), p.second);
				}
			}
		}
		else
		{
			for (pair<string, lua_function> p : funcoes_ponte::funcoes_ponte_map_secoes[set_de_funcoes])
			{
				lua_register(L, p.first.c_str(), p.second);
			}
		}
	}

	int register_function_set(lua_State *L)
	{
		registrar_funcoes_ponte_estado_lua(L, lua_tostring(L, 1));
		return 0;
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
		luaL_openlibs(ret);

		// configurar diretorio
		string lua_path = pegar_local_aplicacao() + "/resources/Scripts/?.lua;" + pegar_local_aplicacao() + "/resources/Scripts/engine_libs/?.lua";

		string link_libs_path = pegar_local_aplicacao() + string("/resources/Scripts/link_libs/?.dll;") + pegar_local_aplicacao() + string("/resources/Scripts/link_libs/?.so");

		lua_getglobal(ret, "package");
		lua_pushstring(ret, lua_path.c_str());
		lua_setfield(ret, -2, "path");
		lua_pushstring(ret, link_libs_path.c_str());
		lua_setfield(ret, -2, "cpath");
		lua_pop(ret, 1);

		lua_newtable(ret);
		for (int i = 0; i < argumentos.size(); i++)
		{
			lua_pushinteger(ret, i);
			lua_pushstring(ret, argumentos[i].c_str());
			lua_settable(ret, -3);
		}
		lua_setglobal(ret, "args");

		// funcoes_lua::adicionar_funcoes_ponte_estado_lua(ret);
		lua_register(ret, "register_function_set", register_function_set);

		int i = luaL_dostring(ret, carregar_script_lua(s)->c_str());

		// buscar erros
		if (i != LUA_OK)
		{
			escrever("LUA NOT OK");

			// get error
			cout << "error in file: " << s << "\n";
			escrever(lua_tostring(ret, -1));
		}

		return ret;
	}

};

class componente_lua : public componente
{
	bool iniciado = false;
	map<string, shared_ptr<string>> scripts_lua_string;
	map<string, bool> scripts_lua_iniciados;

public:
	map<string, lua_State *> estados_lua;

	vector<string> pegar_lista_scripts()
	{
		vector<string> ret = {};
		for (pair<string, lua_State *> p : estados_lua)
		{
			ret.push_back(p.first);
		}
		return ret;
	}

	map<string, lua_State *> pegar_estados_lua()
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
		for (pair<string, lua_State *> p : estados_lua)
		{

			// esse objetoget_tile_set_tile
			lua_pushstring(p.second, ponteiro_string(esse_objeto.get()).c_str());
			lua_setglobal(p.second, "this_object_ptr");

			lua_getglobal(p.second, "START");
			lua_call(p.second, 0, 0);
			scripts_lua_iniciados[p.first] = true;
		}
	}
	void atualisar()
	{

		for (pair<string, lua_State *> p : estados_lua)
		{
			if (scripts_lua_iniciados[p.first])
			{
				lua_State *L = p.second;
				lua_getglobal(L, "UPDATE");
				lua_call(L, 0, 0);
			}
			else
			{
				lua_pushstring(p.second, ponteiro_string(esse_objeto.get()).c_str());
				lua_setglobal(p.second, "this_object_ptr");
				lua_getglobal(p.second, "START");
				lua_call(p.second, 0, 0);
				scripts_lua_iniciados[p.first] = true;
			}
		}
	}
	void colidir(colis_info col)
	{
		for (pair<string, lua_State *> p : estados_lua)
		{
			lua_State *L = p.second;

			if (col.obj != NULL)
			{

				lua_getglobal(L, "COLLIDE");

				lua_pushtable(L, colis_info_table(col));

				lua_call(L, 1, 0);
			}
		}
	}
	void finalisar()
	{
		for (pair<string, lua_State *> p : estados_lua)
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
												{if(add){obj->adicionar_componente<sfml_audio>(sfml_audio());}else{obj->remover_componente<sfml_audio>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("lua_scripts", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<componente_lua>(componente_lua());}else{obj->remover_componente<componente_lua>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_tile_map", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<render_tilemap>(render_tilemap());}else{obj->remover_componente<render_tilemap>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("render_mesh", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<render_malha>(render_malha());}else{obj->remover_componente<render_malha>();} }),
	pair<string, void (*)(objeto_jogo *, bool)>("light", [](objeto_jogo *obj, bool add)
												{if(add){obj->adicionar_componente<fonte_luz>(fonte_luz());}else{obj->remover_componente<fonte_luz>();} }),

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
										  { return obj->tem_componente<sfml_audio>(); }),
	pair<string, bool (*)(objeto_jogo *)>("lua_scripts", [](objeto_jogo *obj)
										  { return obj->tem_componente<componente_lua>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_tile_map", [](objeto_jogo *obj)
										  { return obj->tem_componente<render_tilemap>(); }),
	pair<string, bool (*)(objeto_jogo *)>("render_mesh", [](objeto_jogo *obj)
										  { return obj->tem_componente<render_malha>(); }),
	pair<string, bool (*)(objeto_jogo *)>("light", [](objeto_jogo *obj)
										  { return obj->tem_componente<fonte_luz>(); }),

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

int get_script_name(lua_State *L)
{
	string output = "";
	int argumentos = lua_gettop(L);
	objeto_jogo *obj = NULL;
	if (argumentos > 0)
	{
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2 && obj != NULL && obj->pegar_componente<componente_lua>() != NULL)
	{
		output = obj->pegar_componente<componente_lua>()->pegar_lista_scripts()[0];
	}
	lua_pushstring(L, output.c_str());
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
		map<string, lua_State *> m = cl->pegar_estados_lua();
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
	objeto_jogo *obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	string script_name = lua_tostring(L, 2), var_name = lua_tostring(L, 3);

	lua_getglobal(cl->estados_lua[script_name], var_name.c_str());
	int lua_type_id = lua_type(cl->estados_lua[script_name], -1);
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
		return 0;
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
