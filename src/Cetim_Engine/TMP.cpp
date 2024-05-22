/*
 * AAAAA.cpp
 *
 *  Created on: 19 de jun de 2022
 *      Author: theo
 */

//*

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "implementacao_glfw.h"
#include "compression.h"

void InitImGui()
{
    /*
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(janela, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    */
}

// Function to update the ImGui UI
void UpdateImGuiUI()
{
    /*
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    */
}

void ShutdownImGui()
{
    /*
    ImGui::Begin("Demo window");
    ImGui::Button("Hello!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    */
}
//*/

#include <iostream>
#include <fstream>
#include <functional>
#include <stdlib.h>
#include <thread>
#include <fstream>
#include <type_traits>
#include <iterator>

#include "args.h"

#include "Tempo.h"
#include "LoopPrincipal.h"
#include "Config.h"
#include "Console_Comando.h"
#include "ManusearArquivos.h"
#include "core.h"
#include "box_2d.h"
#include "camera.h"
#include "transform.h"
#include "bullet.h"
#include "input.h"
#include "read_map_file.h"
#include "render_mesh.h"
#include "ui_element.h"
#include <bitset>
#include "table_conversors.h"
#include "compression.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_mixer.h>

#include "ecs/ecs.h"
#include "ecs/ecs_components_systems/ecs_name.h"
#include "ecs/ecs_components_systems/ecs_parents.h"
#include "ecs/ecs_components_systems/ecs_tag.h"

void register_ecs_components()
{

    register_name_component();
    register_family_component();
    register_tag_component();
    transform_ecs::register_transform_component();
    camera_ecs::register_camera_component();
    ecs_render_mesh::register_render_mesh_component();
    ecs_render_sprite::register_render_sprite_component();
}


void configuracaoInicial()
{

    
    
    
    Iniciar_Render_Func.push_back(start_sdl_audio);
    Iniciar_Render_Func.push_back(iniciar_iniciar_global_bullet);
    Iniciar_Render_Func.push_back(start_lua_global_data);
    Iniciar_Render_Func.push_back(terminar_iniciar_global_bullet);
    Iniciar_Render_Func.push_back(iniciarTeste3);

    Antes_Render_Func.push_back(iniciar_atualisar_global_box2D);
    Antes_Render_Func.push_back(atualisar_global_bullet);

    Antes_Render_Func.push_back(update_ui_components);

    Antes_Render_Func.push_back(get_input_using_threads);

    Antes_Render_Func.push_back(terminar_atualisar_global_box2D);

    Antes_Render_Func.push_back(teste3);
}
void comecar()
{

    configuracaoInicial();

    thread temp(Tempo::IniciarTempo);
    temp.detach();

    // gerente_janela = new gerenciador_janela_glfw(true);
    start_window_lib();

    thread grafi(loop_principal::loop_principal);
    // start_input_geter();

    grafi.join();
}

int main(int argc, char **argv)
{

    aplicar_argumentos(argc, argv);

    escrever("argumentos{");
    for (string s : argumentos)
    {
        cout << "	";
        print("    ", s);
    }
    escrever("}");

    if (argumentos.size() > 1)
    {
        setar_diretorio_aplicacao(argumentos[1]);
    }
    else
    {
        setar_diretorio_aplicacao("/home/theo/Cpp/TMP_TECH_DEMO_2D");
    }

    comecar();

    print_benchmark_results();

    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
