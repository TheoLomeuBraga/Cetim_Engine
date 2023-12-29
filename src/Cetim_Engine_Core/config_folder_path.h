

#include <iostream>
#include <string>

#ifdef _WIN64
    #include <windows.h>
    #include <Shlobj.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
#endif

std::string config_folder_path() {
    std::string diretorio;

    #ifdef _WIN64
        PWSTR path;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path))) {
            diretorio = std::wstring(path);
            CoTaskMemFree(path);
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

