
#pragma once
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Config.h"
#include "RecursosT.h"
#include "scene.h"
#include "game_object.h"
#include "transform.h"
#include <filesystem>
#include <thread>
#include <mutex>

bool start_sdl_audio_on = false;
void start_sdl_audio()
{
	if (!start_sdl_audio_on)
	{
		SDL_Init(SDL_INIT_AUDIO);

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
		Mix_AllocateChannels(2);
		start_sdl_audio_on = true;
	}
}

class sdl_sound_manager
{
	Mix_Chunk *buffer = NULL;

public:
	std::string path;
	sdl_sound_manager(std::string path)
	{
		start_sdl_audio();
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

mapeamento_assets<sdl_sound_manager> buffers_som_sdl;

std::set<std::string> sdl_audio_loading_requests_files = {};
std::mutex buffers_som_sdl_mtx;

shared_ptr<sdl_sound_manager> carregar_audio_buffer(string local)
{
	std::lock_guard<std::mutex> lock(buffers_som_sdl_mtx);
	sdl_audio_loading_requests_files.insert(local);
	if (buffers_som_sdl.pegar(local) == NULL)
	{
		buffers_som_sdl.aplicar(local, sdl_sound_manager(local));
	}
	buffers_som_sdl.pegar(local)->get();
	sdl_audio_loading_requests_files.erase(local);
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

class sdl_audio : public componente
{
	shared_ptr<sdl_sound_manager> sound_buffer = NULL;
	int channel;

public:
	audio_info info;

	shared_ptr<transform_> tf = NULL;

	void aplicar_info()
	{
		Mix_HaltChannel(channel);
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

			
			if (distance > info.min_distance && distance <  info.atenuation + info.min_distance)
			{
				Mix_Volume(channel, static_cast<int>((info.volume / 100) * MIX_MAX_VOLUME));
				
			}
			else if (distance < info.min_distance)
			{
				Mix_Volume(channel, static_cast<int>((info.volume / 100) * MIX_MAX_VOLUME));
				
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

	sdl_audio() {}
	sdl_audio(audio_info info)
	{
		this->info = info;
	}

	void finalisar()
	{
		Mix_HaltChannel(channel);
	}
};

/*
#pragma once
#include <iostream>
#include <memory>
#include <SFML/Audio.hpp>
#include "Config.h"
#include "RecursosT.h"
#include "scene.h"
#include "game_object.h"
#include "transform.h"
#include <filesystem>
#include <thread>
#include <mutex>



mapeamento_assets<sf::SoundBuffer> buffers_som_sdl;

std::set<std::string> sdl_audio_loading_requests_files = {};
std::mutex buffers_som_sdl_mtx;

shared_ptr<sf::SoundBuffer> carregar_audio_buffer(string local)
{
	std::lock_guard<std::mutex> lock(buffers_som_sdl_mtx);
	sdl_audio_loading_requests_files.insert(local);
	if (buffers_som_sdl.pegar(local) == NULL)
	{
		buffers_som_sdl.aplicar(local, sf::SoundBuffer());
		buffers_som_sdl.pegar(local)->loadFromFile(local.c_str());
	}
	sdl_audio_loading_requests_files.erase(local);
	return buffers_som_sdl.pegar(local);
}

void carregar_audio_buffer_thread(string local, shared_ptr<sf::SoundBuffer> *ret)
{
	*ret = carregar_audio_buffer(local);
}

shared_ptr<transform_> listener_transform = NULL;

float global_volume_sfml = 0;

void get_set_global_volume_sfml(float vol){
	sf::Listener::setGlobalVolume(vol);
	global_volume_sfml = vol;
}

class sdl_audio : public componente
{
public:
	// https://www.sfml-dev.org/tutorials/2.5/audio-spatialization.php

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

		if (listener_transform != NULL && tf != NULL)
		{

			vec3 lpos = listener_transform->pegar_pos_global();
			vec3 lup = listener_transform->pegar_direcao_local(vec3(0, 1, 0));
			vec3 ldir = listener_transform->pegar_direcao_local(vec3(0, 0, 1));

			sf::Listener::setPosition(sf::Vector3f(lpos.x, lpos.y, lpos.z));
			sf::Listener::setUpVector(sf::Vector3f(lup.x, lup.y, lup.z));
			sf::Listener::setDirection(sf::Vector3f(ldir.x, ldir.y, ldir.z));

			vec3 gpos = tf->pegar_pos_global();
			//som->setPosition(-gpos.x, gpos.y, -gpos.z);
			som->setPosition(sf::Vector3f(gpos.x, gpos.y, gpos.z));

		}
		else
		{
			sf::Listener::setPosition(sf::Vector3f(0,0,0));
			sf::Listener::setUpVector(sf::Vector3f(0,1,0));
			sf::Listener::setDirection(sf::Vector3f(0,0,1));
			som->setPosition(sf::Vector3f(0,0,0));
		}
	}

	sdl_audio(){}
	sdl_audio(audio_info info)
	{
		this->info = info;
	}

	void finalisar()
	{
		som->pause();
	}
};
*/