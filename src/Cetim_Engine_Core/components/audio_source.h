
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

std::set<std::string> audio_source_loading_requests_files = {};

#ifdef USE_SDL

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
bool start_audio_source_on = false;
void start_audio_source()
{
	if (!start_audio_source_on)
	{
		SDL_Init(SDL_INIT_AUDIO);

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
		Mix_AllocateChannels(256);
		start_audio_source_on = true;
	}
}

class sdl_sound_manager
{
	Mix_Chunk *buffer = NULL;

public:
	std::string path;
	sdl_sound_manager(std::string path)
	{
		start_audio_source();
		this->path = path;
	}

	Mix_Chunk *get()
	{
		if (!buffer)
		{
			buffer = Mix_LoadWAV(path.c_str());
		}
		return buffer;
	}

	~sdl_sound_manager()
	{
		if (buffer)
		{
			Mix_FreeChunk(buffer);
		}
	}
};

mapeamento_assets<sdl_sound_manager> buffers_som;

std::mutex buffers_som_sdl_mtx;

shared_ptr<sdl_sound_manager> carregar_audio_buffer(string local)
{
	std::lock_guard<std::mutex> lock(buffers_som_sdl_mtx);
	audio_source_loading_requests_files.insert(local);
	if (buffers_som_sdl.pegar(local) == NULL)
	{
		buffers_som_sdl.aplicar(local, sdl_sound_manager(local));
	}
	buffers_som_sdl.pegar(local)->get();
	audio_source_loading_requests_files.erase(local);
	return buffers_som_sdl.pegar(local);
}

void carregar_audio_buffer_thread(string local, shared_ptr<sdl_sound_manager> *ret)
{
	*ret = carregar_audio_buffer(local);
}

shared_ptr<transform_> listener_transform = NULL;

float global_volume_sfml = 0;

void get_set_global_volume_sfml(float vol)
{
	Mix_VolumeMusic(vol);
	global_volume_sfml = vol;
}

void calcula_panning(float angle_listener_deg, float angle_audio_deg, Uint8 &left, Uint8 &right)
{
	// Lógica para calcular o panning com base nos ângulos
	// Aqui você pode ajustar de acordo com sua lógica específica

	// Exemplo simples: Panning linear entre 0 (esquerda) e 255 (direita)
	float percentagem_pan = (angle_audio_deg - angle_listener_deg + 180.0) / 360.0;
	left = static_cast<Uint8>((1.0 - percentagem_pan) * 255);
	right = static_cast<Uint8>(percentagem_pan * 255);
}

class audio_source : public componente
{
	shared_ptr<sdl_sound_manager> sound_buffer = NULL;
	int channel;

	Uint8 previousLeft = 255;
	Uint8 previousRight = 255;

public:
	audio_info info;

	shared_ptr<transform_> tf = NULL;

	void aplicar_info()
	{

		sound_buffer = carregar_audio_buffer(info.nome);

		if (sound_buffer)
		{

			if (info.loop)
			{
				channel = Mix_PlayChannel(-1, sound_buffer->get(), -1);
			}
			else
			{
				channel = Mix_PlayChannel(-1, sound_buffer->get(), 0);
			}
			Mix_Volume(channel, static_cast<int>((info.volume / 100) * MIX_MAX_VOLUME));
			if (info.pausa)
			{
				Mix_Pause(channel);
			}
			else
			{
				Mix_Resume(channel);
			}
			Mix_SetPanning(channel, 255, 255);
			// Mix_SetDistance(channel,info.min_distance);
		}
		tf = esse_objeto->pegar_componente<transform_>();
	}

	audio_info pegar_info()
	{
		return info;
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

		if (listener_transform != NULL && tf != NULL)
		{
			vec3 pos_lisener = listener_transform->pegar_pos_global();
			vec3 listenerDirection = listener_transform->pegar_direcao_local(vec3(0, 0, -1));
			vec3 pos_audio = tf->pegar_pos_global();
			float distance = glm::distance(pos_lisener, pos_audio) / 2;

			// Defina variáveis globais ou membros da classe para armazenar o estado anterior de panning

			auto SetPanning = [&]()
			{
				vec2 listenerDirection_2d = glm::normalize(vec2(listenerDirection.x, listenerDirection.z));
				glm::vec2 audioDirNormalized = glm::normalize(vec2(pos_audio.x, pos_audio.z) - vec2(pos_lisener.x, pos_lisener.z));

				if (glm::distance(listenerDirection_2d, audioDirNormalized) > 0.1)
				{
					float angle_listener = atan2(listenerDirection_2d.y, listenerDirection_2d.x);
					float angle_audio = atan2(audioDirNormalized.y - listenerDirection_2d.y, audioDirNormalized.x - listenerDirection_2d.x);
					float angle_listener_deg = angle_listener * (180.0 / 3.14159265358979323846);
					float angle_audio_deg = angle_audio * (180.0 / 3.14159265358979323846);

					Uint8 targetLeft, targetRight;
					calcula_panning(angle_listener_deg, angle_audio_deg, targetLeft, targetRight);

					// Ajuste gradual dos valores de panning
					const float smoothingFactor = 0.2; // Ajuste conforme necessário

					Uint8 smoothedLeft = static_cast<Uint8>(previousLeft + smoothingFactor * (targetLeft - previousLeft));
					Uint8 smoothedRight = static_cast<Uint8>(previousRight + smoothingFactor * (targetRight - previousRight));

					Mix_SetPanning(channel, std::min(smoothedRight + (smoothedLeft / 4), 255), std::min(smoothedLeft + (smoothedRight / 4), 255));

					// Atualize os valores anteriores
					previousLeft = smoothedLeft;
					previousRight = smoothedRight;
				}
				else
				{
					Mix_SetPanning(channel, 255, 255);

					// Redefina os valores anteriores quando não há mudança significativa
					previousLeft = 255;
					previousRight = 255;
				}
			};

			if (distance > info.min_distance && distance < info.atenuation + info.min_distance)
			{
				float attenuationFactor = 1.0f - ((distance - info.min_distance) / info.atenuation);
				attenuationFactor = std::max(0.0f, std::min(1.0f, attenuationFactor));
				Mix_Volume(channel, static_cast<int>(attenuationFactor * (info.volume / 100) * MIX_MAX_VOLUME));

				SetPanning();
			}
			else if (distance < info.min_distance)
			{
				Mix_Volume(channel, static_cast<int>((info.volume / 100) * MIX_MAX_VOLUME));

				SetPanning();
			}
			else
			{
				Mix_Volume(channel, 0);
			}
		}
		else
		{
			Mix_SetPanning(channel, 255, 255);
			Mix_Volume(channel, static_cast<int>((info.volume / 100) * MIX_MAX_VOLUME));
		}
	}

	audio_source() {}
	audio_source(audio_info info)
	{
		this->info = info;
	}

	void finalisar()
	{
		Mix_HaltChannel(channel);
	}
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
		else if(som.get())
		{
			som->setPosition(sf::Vector3f(0,0,0));
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
