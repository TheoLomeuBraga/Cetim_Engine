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





#include "Tempo.h"
#include "LoopPrincipal.h"
#include "Config.h"
#include "Console_Comando.h"
#include "ManusearArquivos.h"
#include "Comandos.h"
#include "TESTE.h"
#include "box_2d.h"


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


class A {
public:
    int a;
    shared_ptr<string> teste_memoria;

    int tamanho = 10;

    virtual void falar() { cout << "A\n"; }

    A(){}

    ~A() {
        cout <<"voce deletou A com o valor " << a << endl;
    }
    virtual void* duplicar() {
        return new A(*this);
    }
};
class B : public A {
public:
    int b;
    void falar() { cout << "B\n"; }
    B() {}
    ~B() { cout << "voce deletou B com o valor " << b << endl; }
    void* duplicar() {
        return new B(*this);
    }
};
struct C
{
    int a;
    char b[12];
};

void experimento_ponteiros() {
    shared_ptr<A> a = std::make_shared<A>(A());
    a.get()->a = 7;
    shared_ptr<A> b = std::make_shared<B>(B());
    B* ponteiroB = (B*)b.get();
    ponteiroB->b = 88;

    if (a->teste_memoria != NULL) {
        cout << "resultado teste memoria: " << *a->teste_memoria << endl;
    }
    else {
        cout << "resultado teste memoria falhou \n ";
    }
}




void experimento_ponteiros2() {
    shared_ptr<float> a = make_shared<float>(float(1));
    shared_ptr<float> b = a;
    a = make_shared<float>(float(3));
    cout << *b.get() << endl;
}

void experimento_copia() {
    shared_ptr<A> sp1,sp2;
    sp1 = make_shared<B>(B());
    sp1->falar();
    sp2 = make_shared<A>(*(A*)sp1.get()->duplicar());
    sp2->falar();
}

void experimento_ponteiros3() {
    vector<shared_ptr<float>> fv;
    fv.push_back(make_shared<float>(float(12)));
    shared_ptr<float> f = fv[0];
    cout << f.use_count() << endl;
    fv.clear();
    cout << f.use_count() << endl;
    cout << *f.get() << endl;

}


void experimento_objeto() {
    objeto_jogo obj;
    obj.adicionar_componente<Objetos::transform>(Objetos::transform());
    obj.adicionar_componente<Objetos::camera>(Objetos::camera());
    obj.remover_componente<Objetos::transform>();
    cout << "teste obj componentes: " << obj.pegar_lista_componentes().size() << " " << obj.pegar_lista_componentes()[0] << endl;
}

struct ms
{
    string nome;
    int idade;
};
void exemplo_binario() {
    //escrever
    ms m;
    m.nome = "aurelio";
    m.idade = 12;
    ofstream o("arquivo.data", ios::out | ios::binary);
    o.write((char*)&m, sizeof(ms));
    o.close();

    //ler
    ms m2;
    ifstream i("arquivo.data", ios::out | ios::binary);
    i.read((char*)&m2, sizeof(ms));
    i.close();

    cout << m2.idade << " " << m2.nome << endl;
}

void funcao_tread(string *s) {
    //Sleep(2000);
    *s = "ola";
}

void teste_soma_tread(float a,float b,float* ret) {
    *ret = a + b;
}

int main(int argc, char** argv)
{

    //float ret = 0;
    //thread t(teste_soma_tread,2,2,&ret);
    //t.join();
    //cout << ret << endl;




    aplicar_argumentos(argc, argv);
    cout << "vamos ao trabalho\n";
    comecar();
    cout << "volte sempre\n";



    return 0;
}
