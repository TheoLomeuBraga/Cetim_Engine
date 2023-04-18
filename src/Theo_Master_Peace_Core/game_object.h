#pragma once

#include <iostream>
#include <functional>
using namespace std;

#include "RecursosT.h"

class objeto_jogo;

class componente
{
public:
	shared_ptr<objeto_jogo> esse_objeto = NULL;
	componente() {}
	virtual void iniciar() {}
	virtual void atualisar() {}
	virtual void colidir(colis_info col) {}
	virtual void finalisar() {}
};

class objeto_jogo
{
private:
	shared_ptr<objeto_jogo> esse_objeto;
	map<string, shared_ptr<componente>> componentes;

public:
	static int instancias;
	void setar_esse_objeto(shared_ptr<objeto_jogo> objeto)
	{
		esse_objeto = objeto;
	}

	bool lixo = false;
	int ID;
	string nome;
	bool em_cena = false;

	objeto_jogo *pai;
	vector<shared_ptr<objeto_jogo>> filhos;
	vector<objeto_jogo *> childrens_to_remove;

	bool is_in_childrens_to_remove(objeto_jogo *children)
	{
		for (objeto_jogo *c : childrens_to_remove)
		{
			if (c == children)
			{
				return true;
			}
		}
		return false;
	}

	void remove_childrens_to_remove()
	{
		for (shared_ptr<objeto_jogo> children : filhos)
		{
			children->remove_childrens_to_remove();
		}
		vector<shared_ptr<objeto_jogo>> new_children_list = {};
		for (shared_ptr<objeto_jogo> children : filhos)
		{
			if (!is_in_childrens_to_remove(children.get()))
			{
				new_children_list.push_back(children);
			}
		}
		filhos.swap(new_children_list);
		vector<objeto_jogo *> new_childrens_to_remove_list = {};
		childrens_to_remove.swap(new_childrens_to_remove_list);
	}

	void limpar_lixo()
	{
		map<string, shared_ptr<componente>> componentesB;
		for (pair<string, shared_ptr<componente>> p : componentes)
		{
			componentesB.insert(p);
		}
		componentes.swap(componentesB);

		vector<shared_ptr<objeto_jogo>> filhosB;
		for (int i = 0; i < filhos.size(); i++)
		{
			filhosB.resize(filhos.size());
			filhosB[i] = filhos[i];
		}
		filhos.swap(filhosB);
	}

	void desconectar_componentes()
	{
		esse_objeto = NULL;

		for (pair<string, shared_ptr<componente>> p : componentes)
		{
			p.second->esse_objeto = NULL;
		}
	}

	objeto_jogo()
	{
		instancias += 1;
	}

	objeto_jogo(bool en_cena)
	{
		instancias += 1;
		this->em_cena = em_cena;
	}

	objeto_jogo(string t)
	{
		instancias += 1;
		nome = t;
	}

	void excluir()
	{
		for (shared_ptr<objeto_jogo> obj : filhos)
		{
			obj->excluir();
		}
		lixo = true;
	}

	template <typename X>
	void adicionar_componente(string nome, X comp)
	{
		limpar_lixo();
		comp.esse_objeto = esse_objeto;
		if (componentes.find(nome) == componentes.end())
		{
			componentes.insert(pair<string, shared_ptr<componente>>(nome, std::make_shared<X>(comp)));
			if (em_cena)
			{
				componentes[nome]->iniciar();
			}
		}
	}
	template <typename X>
	void adicionar_componente(X comp)
	{
		adicionar_componente(pegar_tipo_variavel<X>(), comp);
	}
	template <typename X>
	void adicionar_componente()
	{
		adicionar_componente<X>(X());
	}

	template <typename X>
	shared_ptr<X> pegar_componente()
	{
		string nome_tipo = pegar_tipo_variavel<X>();
		if (componentes.find(nome_tipo) != componentes.end())
		{
			// return componentes[nome_tipo];
			return dynamic_pointer_cast<X>(componentes[nome_tipo]);
		}
		else
		{
			return NULL;
		}
	}

	template <typename X>
	bool tem_componente()
	{
		shared_ptr<componente> c = pegar_componente<X>();
		if (c = NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void remover_componente(string s)
	{
		limpar_lixo();
		if (componentes.find(s) != componentes.end())
		{
			if (em_cena)
			{
				componentes[s]->finalisar();
			}
			componentes[s]->esse_objeto = NULL;
			map<string, shared_ptr<componente>> componentes2;

			for (pair<string, shared_ptr<componente>> p : componentes)
			{
				if (p.first.compare(s))
				{
					componentes2.insert(p);
				}
			}

			componentes.swap(componentes2);
			// componentes.erase(s);
		}
	}

	template <typename X>
	void remover_componente()
	{
		remover_componente(typeid(X).name());
	}

	vector<string> pegar_lista_componentes()
	{
		vector<string> ret;

		for (pair<string, shared_ptr<componente>> c : componentes)
		{
			ret.push_back(c.first);
		}

		return ret;
	}

	void limpar_componentes()
	{
		componentes.clear();
		for (pair<string, shared_ptr<componente>> p : componentes)
		{
			remover_componente(p.first);
		}
		map<string, shared_ptr<componente>> vazio;
		componentes.swap(vazio);
	}

	void limpar_parentesco()
	{
		pai = NULL;
		vector<shared_ptr<objeto_jogo>> vazios;
		filhos.swap(vazios);
	}

	~objeto_jogo()
	{
		instancias -= 1;
		limpar_componentes();
		limpar_parentesco();

		limpar_lixo();
	}

	void marcar_em_cena_como(bool b)
	{
		em_cena = b;
		for (shared_ptr<objeto_jogo> obj : filhos)
		{
			obj->marcar_em_cena_como(b);
		}
	}

	void iniciar()
	{
		em_cena = true;

		for (pair<string, shared_ptr<componente>> p : componentes)
		{
			p.second->esse_objeto = esse_objeto;
			p.second->iniciar();
		}
	}

	void atualisar()
	{

		if (em_cena)
		{
			limpar_lixo();

			for (pair<string, shared_ptr<componente>> p : componentes)
			{
				p.second->atualisar();
			}
		}
	}

	void colidir(colis_info col)
	{
		if (em_cena)
		{

			for (pair<string, shared_ptr<componente>> p : componentes)
			{
				p.second->colidir(col);
			}
		}
	}

	void finalisar()
	{
		em_cena = false;

		for (pair<string, shared_ptr<componente>> p : componentes)
		{

			p.second->finalisar();
		}
	}

	void adicionar_cena()
	{
		iniciar();

		for (shared_ptr<objeto_jogo> obj : filhos)
		{
			obj->adicionar_cena();
		}
	}
};
int objeto_jogo::instancias = 0;

shared_ptr<objeto_jogo> novo_objeto_jogo()
{
	shared_ptr<objeto_jogo> ret = make_shared<objeto_jogo>();
	ret->setar_esse_objeto(ret);
	return ret;
}
shared_ptr<objeto_jogo> novo_objeto_jogo(bool em_cena)
{
	shared_ptr<objeto_jogo> ret = make_shared<objeto_jogo>(objeto_jogo(em_cena));
	ret->setar_esse_objeto(ret);
	return ret;
}