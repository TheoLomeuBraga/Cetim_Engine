#include <iostream>
#include <fstream>
#include "lua/lua.hpp"

bool CompileLuaScript(const char* inputScriptPath, const char* outputBytecodePath) {
    lua_State* L = luaL_newstate();
    if (!L) {
        return false; // Failed to create Lua state
    }

    luaL_openlibs(L);

    int result = luaL_loadfile(L, inputScriptPath);
    if (result != LUA_OK) {
        lua_close(L);
        return false; // Failed to load the script file
    }

    std::ofstream outputBytecode(outputBytecodePath, std::ios::binary);
    if (!outputBytecode) {
        lua_close(L);
        return false; // Failed to open the bytecode output file
    }

    result = lua_dump(L, [](lua_State*, const void* p, size_t sz, void* ud) {
        std::ofstream* output = static_cast<std::ofstream*>(ud);
        output->write(static_cast<const char*>(p), sz);
        return 0;
    }, &outputBytecode, 0); // Pass 0 as the last argument

    lua_close(L);

    if (result != LUA_OK) {
        return false; // Failed to compile and save the bytecode
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: lua_compiler <script_lua> <bytecode_output>" << std::endl;
        return 1;
    }

    const char* inputScriptPath = argv[1];
    const char* outputBytecodePath = argv[2];

    if (CompileLuaScript(inputScriptPath, outputBytecodePath)) {
        std::cout << "Lua script compiled successfully: " << inputScriptPath << " -> " << outputBytecodePath << std::endl;
        return 0;
    } else {
        std::cout << "Failed to compile Lua script." << std::endl;
        return 2;
    }
}
