#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EPATH
#define EPATH "./build/cetim_engine"
#endif

#ifndef CPATH
#define CPATH "./projects/HMTF"
#endif

#ifndef LPATHL
#define LPATHL "./x86_64/linux"
#endif

#ifndef LPATHW
#define LPATHW "./x86_64/windows/dll"
#endif

#define MAX_BUFFER_SIZE 1024

char *run_command(const char *comando)
{
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    char *saida = NULL;
    size_t tamanho_saida = 0;

    /* Execute o comando em um processo */
    fp = popen(comando, "r");
    if (fp == NULL)
    {
        printf("Erro ao executar o comando\n");
        return NULL;
    }

    /* Leia a saída do comando e armazene em uma string */
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        tamanho_saida += strlen(buffer);
        saida = realloc(saida, tamanho_saida + 1);
        strcat(saida, buffer);
    }

    /* Feche o ponteiro de arquivo */
    pclose(fp);

    return saida;
}

void single_line(char *str)
{
    int i, j = 0;
    for (i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] != '\n')
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0'; // Adicionar o terminador nulo
}

#ifdef _WIN32
#include <Windows.h>
int main()
{

    char current_directory[256] = "";
    if (GetCurrentDirectory(256, current_directory) == 0) {
        printf("Erro ao obter o diretório atual\n");
        return 1;
    }

    char cmd[256] = "";
    strcat(cmd, current_directory);
    strcat(cmd, LPATHW);

    single_line(&cmd);
    run_command(cmd);;
    if (!SetEnvironmentVariable("PATH", cmd)) {
        perror("Erro ao definir variável de ambiente");
        return 1;
    }

    // play
    char cmd2[256] = "";
    strcat(cmd2, current_directory);
    strcat(cmd2, EPATH);
    strcat(cmd2, ".exe ");
    strcat(cmd2, current_directory);
    strcat(cmd2, CPATH);

    single_line(&cmd2);
    puts(cmd2);
    system(cmd2);

    return 0;
}
#endif

#ifdef __unix__
int main()
{

    char current_directory[256] = "";
    strcpy(current_directory, run_command("pwd"));

    char cmd[256] = "";
    strcat(cmd, current_directory);
    strcat(cmd, LPATHL);

    single_line(&cmd);
    run_command(cmd);;
    if (setenv("LD_LIBRARY_PATH", cmd, 1) != 0) {
        perror("Erro ao definir LD_LIBRARY_PATH");
        return 1;
    }

    // play
    char cmd2[256] = "";
    strcat(cmd2, current_directory);
    strcat(cmd2, EPATH);
    strcat(cmd2, " ");
    strcat(cmd2, current_directory);
    strcat(cmd2, CPATH);

    single_line(&cmd2);
    puts(cmd2);
    system(cmd2);

    return 0;
}
#endif
