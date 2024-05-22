
#pragma once
#include <iostream>
#include <memory>

#include "Config.h"
#include "RecursosT.h"
#include "scene.h"
#include "game_object.h"
#include "transform.h"
#include <filesystem>
#include <thread>
#include <mutex>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

std::set<std::string> audio_source_loading_requests_files = {};

#define USE_SDL

#ifdef USE_SDL

#include <SDL3/SDL.h>
#include <SDL3/SDL_mixer.h>

SDL_AudioSpec spec;
void start_sdl_audio()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	//spec.freq = 44100;
	//spec.format = SDL_AUDIO_S16LE;
	spec.freq = 16000;
	spec.format = SDL_AUDIO_S8;
	spec.channels = 1;
	Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, &spec);
	Mix_AllocateChannels(255);
}

mapeamento_assets<Mix_Chunk> buffers_som;
std::set<std::string> sdl_audio_loading_requests_files = {};
std::mutex buffers_som_mtx;

std::shared_ptr<Mix_Chunk> carregar_audio_buffer(std::string local)
{
	std::lock_guard<std::mutex> lock(buffers_som_mtx);
	sdl_audio_loading_requests_files.insert(local);
	if (buffers_som.pegar(local) == nullptr)
	{
		auto chunk = Mix_LoadWAV(local.c_str());
		if (chunk != nullptr)
		{
			buffers_som.aplicar_ptr(local, std::shared_ptr<Mix_Chunk>(chunk, Mix_FreeChunk));
		}
	}
	sdl_audio_loading_requests_files.erase(local);
	return buffers_som.pegar(local);
}

void carregar_audio_buffer_thread(std::string local, std::shared_ptr<Mix_Chunk> *ret)
{
	*ret = carregar_audio_buffer(local);
}

std::shared_ptr<transform_> listener_transform = nullptr;
float global_volume_sdl = 0;

void get_set_global_volume_sdl(float vol)
{

	global_volume_sdl = vol;
	if (vol * MIX_MAX_VOLUME > 0)
	{
		Mix_Volume(-1, static_cast<int>(vol * MIX_MAX_VOLUME / 100.0f));
		return;
	}
	Mix_Volume(-1, 0);
}

class audio_source : public componente
{
private:
	float volume = 0;

public:
	struct audio_info
	{
		std::string nome;
		float tempo = 0;
		float volume = 100;
		bool loop = false;
		float velocidade = 1.0f;
		float min_distance = 1.0f;
		float atenuation = 1.0f;
		bool pausa = false;
	} info;

	std::shared_ptr<Mix_Chunk> buffer;
	int channel = -1;
	std::shared_ptr<transform_> tf = NULL;

	audio_source() {}
	audio_source(audio_info info) : info(info) {}

	void aplicar_info()
	{
		buffer = carregar_audio_buffer(info.nome);
		if (buffer != nullptr)
		{
			channel = Mix_PlayChannel(-1, buffer.get(), info.loop ? -1 : 0);
			if (channel != -1)
			{
				Mix_Volume(channel, static_cast<int>(info.volume * MIX_MAX_VOLUME / 100.0f));
				volume = info.volume;
				Mix_Pause(channel);
				if (!info.pausa)
				{
					Mix_Resume(channel);
				}
			}
		}
		tf = esse_objeto->pegar_componente<transform_>();
	}

	audio_info pegar_info()
	{
		audio_info ret = info;
		if (channel != -1)
		{
			ret.tempo = Mix_GetChunk(channel)->alen;
			ret.volume = volume;
			ret.loop = Mix_Playing(channel) ? true : false;
		}
		return ret;
	}

	void iniciar()
	{
		// aplicar_info();
	}

	float calcular_rotacao_necessaria(const glm::vec3 &ponto_a, const glm::vec3 &ponto_b, const glm::vec3 &direcao)
	{
		// Calcular a direção de A para B
		glm::vec3 direcao_a_para_b = glm::normalize(ponto_b - ponto_a);

		// Calcular o ângulo em relação ao eixo z (assumindo que a rotação inicial é em torno de y)
		float angulo_direcao_a_para_b = glm::degrees(atan2(direcao_a_para_b.x, direcao_a_para_b.z));

		// Calcular o ângulo da direção atual
		float angulo_direcao_atual = glm::degrees(atan2(direcao.x, direcao.z));

		// Calcular a rotação necessária
		float rotacao_necessaria = angulo_direcao_a_para_b - angulo_direcao_atual;

		// Normalizar a rotação para o intervalo [-180, 180)
		/*
		if (rotacao_necessaria > 180.0f)
		{
			rotacao_necessaria -= 360.0f;
		}
		else if (rotacao_necessaria < -180.0f)
		{
			rotacao_necessaria += 360.0f;
		}
		*/

		return rotacao_necessaria;
	}

	void atualisar()
	{
		if (channel != -1 && listener_transform != nullptr && tf != nullptr)
		{
			glm::vec3 lpos = listener_transform->pegar_pos_global();
			glm::vec3 gpos = tf->pegar_pos_global();

			// Calcular vetor da posição do ouvinte para a posição da fonte sonora
			glm::vec3 direction = gpos - lpos;

			// Calcular ângulo em relação ao ouvinte
			vec3 tf_dir = listener_transform->pegar_direcao_local(vec3(0, 0, -1));
			float angle = calcular_rotacao_necessaria(lpos, gpos, tf_dir);

			// Calcular distância
			float distance = glm::length(direction);
			if (distance < 1.0f)
			{
				distance = 0;
			}

			// Normalizar a distância para o intervalo de 0 a 255
			Uint8 sdl_distance = static_cast<Uint8>(glm::clamp(distance / 10.0f * 255.0f, 0.0f, 255.0f)); // Ajustar a escala de distância conforme necessário

			if (distance == 0)
			{
				Mix_SetPosition(channel, 0, 0);
			}
			else if (Mix_SetPosition(channel, static_cast<Sint16>(angle), sdl_distance) == 0)
			{
				printf("Mix_SetPosition: %s\n", Mix_GetError());
			}
		}
		else if (channel != -1)
		{
			// Posição padrão se nenhuma transformação estiver disponível
			if (Mix_SetPosition(channel, 0, 0) == 0)
			{
				printf("Mix_SetPosition: %s\n", Mix_GetError());
			}
		}
	}

	// void finalisar() {
	//     if (channel != -1) {
	//         Mix_HaltChannel(channel);
	//     }
	// }
};

#else

#include <SFML/Audio.hpp>

mapeamento_assets<sf::SoundBuffer> buffers_som;

std::set<std::string> sfml_audio_loading_requests_files = {};
std::mutex buffers_som_mtx;

shared_ptr<sf::SoundBuffer> carregar_audio_buffer(string local)
{
	std::lock_guard<std::mutex> lock(buffers_som_mtx);
	sfml_audio_loading_requests_files.insert(local);
	if (buffers_som.pegar(local) == NULL)
	{
		buffers_som.aplicar(local, sf::SoundBuffer());
		buffers_som.pegar(local)->loadFromFile(local.c_str());
	}
	sfml_audio_loading_requests_files.erase(local);
	return buffers_som.pegar(local);
}

void carregar_audio_buffer_thread(string local, shared_ptr<sf::SoundBuffer> *ret)
{
	*ret = carregar_audio_buffer(local);
}

shared_ptr<transform_> listener_transform = NULL;

float global_volume_sfml = 0;

void get_set_global_volume_sfml(float vol)
{
	sf::Listener::setGlobalVolume(vol);
	global_volume_sfml = vol;
}

class audio_source : public componente
{
public:
	audio_info info;
	shared_ptr<sf::Sound> som;
	shared_ptr<sf::SoundBuffer> buffer;

	shared_ptr<transform_> tf = NULL;

	void aplicar_info()
	{

		buffer = carregar_audio_buffer(info.nome);
		som = make_shared<sf::Sound>(*buffer.get());
		som->setBuffer(*buffer.get());

		som->setPlayingOffset(sf::seconds(info.tempo));
		som->setVolume(info.volume);
		som->setLoop(info.loop);
		som->setPitch(info.velocidade);
		som->setMinDistance(info.min_distance);
		som->setAttenuation(info.atenuation);
		if (info.pausa)
		{
			som->pause();
		}
		else
		{
			som->play();
		}
		tf = esse_objeto->pegar_componente<transform_>();
	}

	audio_info pegar_info()
	{
		audio_info ret;

		ret.tempo = som->getPlayingOffset().asSeconds();
		ret.volume = som->getVolume();
		ret.loop = som->getLoop();
		ret.velocidade = som->getPitch();
		ret.min_distance = som->getMinDistance();
		ret.atenuation = som->getAttenuation();

		return ret;
	}

	int iniciado = 0;
	void iniciar()
	{
		// aplicar_info();
	}

	void atualisar()
	{
		// cout << pegar_info().volume << endl;

		tf = esse_objeto->pegar_componente<transform_>();

		if (som.get() && listener_transform != NULL && tf != NULL)
		{

			vec3 lpos = listener_transform->pegar_pos_global();
			vec3 lup = listener_transform->pegar_direcao_local(vec3(0, 1, 0));
			vec3 ldir = listener_transform->pegar_direcao_local(vec3(0, 0, 1));

			sf::Listener::setPosition(sf::Vector3f(lpos.x, lpos.y, lpos.z));
			sf::Listener::setUpVector(sf::Vector3f(lup.x, lup.y, lup.z));
			sf::Listener::setDirection(sf::Vector3f(ldir.x, ldir.y, ldir.z));

			vec3 gpos = tf->pegar_pos_global();
			som->setPosition(sf::Vector3f(gpos.x, gpos.y, gpos.z));
		}
		else if (som.get())
		{
			som->setPosition(sf::Vector3f(0, 0, 0));
		}
	}

	audio_source() {}
	audio_source(audio_info info)
	{
		this->info = info;
	}

	/*
	void finalisar()
	{
		if(som.get()){
			som->stop();
		}

	}
	*/
};
#endif
