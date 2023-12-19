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



mapeamento_assets<sf::SoundBuffer> buffers_som_sfml;

std::set<std::string> sfml_loading_requests_files = {};
std::mutex buffers_som_sfml_mtx;

shared_ptr<sf::SoundBuffer> carregar_audio_buffer_sfml(string local)
{
	std::lock_guard<std::mutex> lock(buffers_som_sfml_mtx);
	sfml_loading_requests_files.insert(local);
	if (buffers_som_sfml.pegar(local) == NULL)
	{
		buffers_som_sfml.aplicar(local, sf::SoundBuffer());
		buffers_som_sfml.pegar(local)->loadFromFile(local.c_str());
	}
	sfml_loading_requests_files.erase(local);
	return buffers_som_sfml.pegar(local);
}

void carregar_audio_buffer_thread(string local, shared_ptr<sf::SoundBuffer> *ret)
{
	*ret = carregar_audio_buffer_sfml(local);
}

shared_ptr<transform_> listener_transform = NULL;

float global_volume_sfml = 0;

void get_set_global_volume_sfml(float vol){
	sf::Listener::setGlobalVolume(vol);
	global_volume_sfml = vol;
}

class sfml_audio : public componente
{
public:
	// https://www.sfml-dev.org/tutorials/2.5/audio-spatialization.php

	audio_info info;
	shared_ptr<sf::Sound> som;
	shared_ptr<sf::SoundBuffer> buffer;

	shared_ptr<transform_> tf = NULL;

	void aplicar_info()
	{

		buffer = carregar_audio_buffer_sfml(info.nome);
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

	sfml_audio(){}
	sfml_audio(audio_info info)
	{
		this->info = info;
	}

	void finalisar()
	{
		som->pause();
	}
};
