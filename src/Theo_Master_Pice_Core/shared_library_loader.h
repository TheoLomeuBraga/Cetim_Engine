#include <string>

typedef void* (*Shared_Library_Loader_Function)(void*);

#ifdef WINDOWS
#include <windows.h>
class Shared_Library_Loader
{
public:
    HMODULE hDLL;
    Shared_Library_Loader(std::string lib_name)
    {
        hDLL = LoadLibrary((lib_name + std::string(".dll")).c_str());
    }
    void *call_function(std::string function_name, void *arg)
    {
        Shared_Library_Loader_Function MyFunction = reinterpret_cast<Shared_Library_Loader_Function>(GetProcAddress(hDLL, function_name.c_str()));


        if (MyFunction != nullptr)
        {
            return MyFunction(arg);
        }
        return 0;
    }
    ~Shared_Library_Loader()
    {
        FreeLibrary(hDLL);
    }
};
#endif

#ifdef UNIX
#include <dlfcn.h>
class Shared_Library_Loader
{
public:
    void *handle;
    Shared_Library_Loader(std::string lib_name)
    {
        handle = dlopen(( std::string("./") + lib_name + std::string(".so")).c_str(), RTLD_LAZY);
    }
    void *call_function(std::string function_name, void *arg)
    {
        Shared_Library_Loader_Function MyFunction = reinterpret_cast<Shared_Library_Loader_Function>(dlsym(handle, function_name.c_str()));

        if (MyFunction != nullptr)
        {
            return MyFunction(arg);
        }
        return 0;
    }
    ~Shared_Library_Loader()
    {
        dlclose(handle);
    }
};
#endif
