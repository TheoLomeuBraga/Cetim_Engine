#pragma once
#include <string>
#include <unordered_map>
#include <map>

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

struct LuaFunctionWrapper{
    lua_State *L;        // Estado Lua
    std::string functionRef;     // Referência da função Lua no registrador
};
typedef struct LuaFunctionWrapper LuaFunctionWrapper;

class Table {
public:
    std::unordered_map<std::string, float> m_floatMap;
    std::unordered_map<std::string, std::string> m_stringMap;
    std::map<std::string, Table> m_tableMap;
    std::map<std::string, lua_CFunction> m_lua_functionMap;

    bool haveFloat(const std::string& key){
        if(m_floatMap.find(key) != m_floatMap.end()){return true;}else{return false;}
    }

    bool haveString(const std::string& key){
        if(m_stringMap.find(key) != m_stringMap.end()){return true;}else{return false;}
    }

    bool haveTable(const std::string& key){
        if(m_tableMap.find(key) != m_tableMap.end()){return true;}else{return false;}
    }

    bool haveLuaTable(const std::string& key){
        if(m_lua_functionMap.find(key) != m_lua_functionMap.end()){return true;}else{return false;}
    }

    void setFloat(const std::string& key, float value) {
        m_floatMap[key] = value;
    }

    float getFloat(const std::string& key) {
        return m_floatMap[key];
    }

    void setString(const std::string& key, const std::string& value) {
        m_stringMap[key] = value;
    }

    std::string getString(const std::string& key) {
        return m_stringMap[key];
    }

    void setTable(const std::string& key, const Table& table) {
        m_tableMap[key] = table;
    }

    Table getTable(const std::string& key) {
        return m_tableMap[key];
    }

    void setLuaFunction(const std::string& key, lua_CFunction& func) {
        m_lua_functionMap[key] = func;
    }

    lua_CFunction getLuaFunction(const std::string& key) {
        return m_lua_functionMap[key];
    }


    
};