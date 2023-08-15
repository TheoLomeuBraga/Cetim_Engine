#pragma once

#include <iostream>
#include <chrono>
#include <thread>

#include "Config.h"

namespace Tempo
{

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

	class Timer
	{
	private:
		using clock = std::chrono::high_resolution_clock;

		std::chrono::time_point<clock> start_time;
		std::chrono::duration<double> accumulated_time;
		bool running;

	public:
		Timer() : running(false), accumulated_time(0.0) {}

		void start()
		{
			if (!running)
			{
				start_time = clock::now();
				running = true;
			}
		}

		void stop()
		{
			if (running)
			{
				accumulated_time += clock::now() - start_time;
				running = false;
			}
		}

		void resume()
		{
			if (!running)
			{
				start_time = clock::now();
				running = true;
			}
		}

		double get() const
		{
			if (running)
			{
				auto current_time = clock::now();
				return accumulated_time.count() + std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time).count();
			}
			else
			{
				return accumulated_time.count();
			}
		}
	};

}
