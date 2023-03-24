/*
 * AAAAA.cpp
 *
 *  Created on: 19 de jun de 2022
 *      Author: theo
 */




#include <iostream>
#include <fstream>
#include <functional>
#include <stdlib.h>
#include <thread>
#include <future>
#include  <fstream>
#include <type_traits>
#include <iterator>


#include "args.h"


#include "Tempo.h"
#include "LoopPrincipal.h"
#include "Config.h"
#include "Console_Comando.h"
#include "ManusearArquivos.h"
#include "TESTE.h"
#include "components/box_2d.h"
#include "components/camera.h"
#include "components/transform.h"

#include "read_map_file.h"

#include <bitset>







void configuracaoInicial(){











    Iniciar_Render_Func.push_back(iniciarTeste3);
    Antes_Render_Func.push_back(atualisar_global_box2D);
    Antes_Render_Func.push_back(teste3);

   //Iniciar_Render_Func.push_back(inciar_imgui);
   //Antes_Render_Func.push_back(antes_imgui);
   //Depois_Render_Func.push_back(depois_imgui);


}
void comecar() {


    configuracaoInicial();

    thread temp(Tempo::IniciarTempo);


    gerente_janela = new gerenciador_janela_glfw(true);

    thread grafi(loop_principal::loop_principal);
    //thread grafi(loop_principal::IniciarJanela);
    manuseio_inputs = new glfw_input_manager();
    thread inpu(loop_input);
    //thread fisi(Fisica::iniciar_fisica);

    grafi.join();


}


int main(int argc, char** argv)
{


    

	escrever(pegar_estencao_arquivo("arquivo.abc"));
    aplicar_argumentos(argc, argv);

    escrever("argumentos{");
    for(string s : argumentos){
    	cout << "	";
    	escrever(s);
    }
    escrever("}");

    if(argumentos.size() > 1){
    	setar_diretorio_aplicacao(argumentos[1]);
    }
    else{
    	setar_diretorio_aplicacao("/home/theo/Cpp/TMP_TECH_DEMO_2D");
    }

    cout << "vamos ao trabalho\n";
    escrever(pegar_local_aplicacao());




    test_read_map_file();
    comecar();
    cout << "volte sempre\n";



    return 0;
}
