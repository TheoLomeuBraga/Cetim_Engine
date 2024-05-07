#pragma once

#include <iostream>
#include <chrono>
#include <thread>

#include "Config.h"

namespace Tempo
{


	const unsigned int target_frames = 120;
	const long double time_step = 1.0 / (long double)target_frames;
	long double deltaTime = 0;

	

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

		long double get()
		{
			auto end_time = clock::now();
			std::chrono::duration<long double> elapsed_seconds = end_time - start_time;
			return elapsed_seconds.count();
		}
	};

	class HDTimer
	{
	private:
		using clock = std::chrono::high_resolution_clock;

		std::chrono::time_point<clock> start_time;

	public:
		HDTimer() : start_time(clock::now()) {}

		void clear()
		{
			start_time = clock::now();
		}

		long double get()
		{
			auto end_time = clock::now();
			std::chrono::duration<long double> elapsed_seconds = end_time - start_time;
			return elapsed_seconds.count();
		}
	};

	Timer total_timer;
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
