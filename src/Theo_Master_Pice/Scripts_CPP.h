#pragma once
#include <iostream>
using namespace std;
#include "Objetos.h"
using namespace Objetos;
using namespace Tempo;
using namespace input;



#include "box_2d.h"



class exemplo : public componente {
public:
	//objeto_jogo* esse_objeto;
	//bool iniciado = false;
	void iniciar() {}
	void atualisar() {}
	void colidir(colis_info col) {}
	void finalisar() {}
};






namespace Scripts_CPP {

	class colisDEBUG : public componente {
		bool b = false;
		colisDEBUG() {}

		
		void atualizar() {
		}

		void ao_colidir(colis_info colli) {
		}

		

	};

	class teste_raio : public componente {


		

		teste_raio() {}
		colis_info rc_info;

		



		

	};

	class controle : public componente {
		
	public:

		controle() {}
		
		
		

		colis_info rc_info;

		void iniciar() {
			cout << "personajem iniciado\n";
			

			
		}

		void atualisar() {
			
		}


		void colidir(colis_info colli) {
			
			
		}
		

		

	};


	class camera_script : public componente {
	public:
		camera_script(){}
		objeto_jogo* personajem;

		void atualisar() {
			if (personajem != NULL) {
				esse_objeto->pegar_componente<Objetos::transform>()->pos = vec3(personajem->pegar_componente<Objetos::transform>()->pos.x, personajem->pegar_componente<Objetos::transform>()->pos.y + 1, -1);
			}
		}

	};


};
