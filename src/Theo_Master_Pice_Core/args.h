#pragma once
#include <string>
#include <vector>

std::vector<std::string> argumentos;

void aplicar_argumentos(int argc, char** argv) {
	argumentos.resize(argc);
	for (int i = 0; i < argc; i++) {
		argumentos[i] = argv[i];
	}
}