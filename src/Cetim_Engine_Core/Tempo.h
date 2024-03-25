#pragma once

#include <iostream>
#include <chrono>
#include <thread>

#include "Config.h"

namespace Tempo
{

	/*
	class Timer
	{
	private:
		using clock = std::chrono::high_resolution_clock;

		std::chrono::time_point<clock> start_time;

	public:
		Timer()
		{
			start_time = clock::now();
		}

		void clear()
		{
			start_time = clock::now();
		}

		double get()
		{
			return std::chrono::duration_cast<std::chrono::duration<double>>(clock::now() - start_time).count();
		}
	};
	*/

	class Timer
	{
	private:
		using clock = std::chrono::steady_clock;

		std::chrono::time_point<clock> start_time;

	public:
		Timer() : start_time(clock::now()) {}

		void clear()
		{
			start_time = clock::now();
		}

		double get()
		{
			auto end_time = clock::now();
			std::chrono::duration<long double> elapsed_seconds = end_time - start_time;
			return elapsed_seconds.count();
		}
	};

	double tempo;
	double velocidadeTempo = 1;

	int FPS;

	double tempUltFrameEntidades, varTempEntidades;
	double tempUltFrameRender, varTempRender;
	double tempUltFrame, varInputTemp;

	bool depurar_FPS = true;

	chrono::steady_clock::time_point inicio;
	chrono::steady_clock::time_point atual;

	void ContinuarTempo()
	{
		atual = chrono::steady_clock::now();
		chrono::duration<double> t = atual - inicio;
		tempo = t.count();
		FPS = 1 / varTempRender;
	}

	void IniciarTempo()
	{
		cout << "Relogio Interno Online" << endl;

		inicio = chrono::steady_clock::now();
		while (true)
		{
			if (configuracoes::desligar)
			{
				break;
			}
			ContinuarTempo();
		}
	}

}
