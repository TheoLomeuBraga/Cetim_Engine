

#include <iostream>
#include <string>

#ifdef _WIN64
#define WIN32_LEAN_AND_MEAN
    #include <windows.h>

#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
#endif

std::string config_folder_path() {
    std::string diretorio;

    #ifdef _WIN64
       const char *appData = getenv("APPDATA");
        if (appData != nullptr) {
            diretorio = appData;
        } else {
            std::cerr << "Erro ao obter o diretório Roaming no Windows." << std::endl;
        }
    #else
        const char *xdgConfigHome = getenv("XDG_CONFIG_HOME");
        if (xdgConfigHome != nullptr) {
            diretorio = xdgConfigHome;
        } else {
            struct passwd *pw = getpwuid(getuid());
            if (pw != nullptr) {
                diretorio = pw->pw_dir;
                diretorio += "/.config";
            } else {
                std::cerr << "Erro ao obter o diretório de configuração no Linux." << std::endl;
            }
        }
    #endif

    return diretorio;
}

