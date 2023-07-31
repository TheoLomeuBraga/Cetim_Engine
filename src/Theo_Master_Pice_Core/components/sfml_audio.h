#pragma once
#include <iostream>
#include <memory>
#include <SFML/Audio.hpp>
#include "Config.h"
#include "RecursosT.h"
#include "scene.h"
#include "game_object.h"
#include "transform.h"

mapeamento_assets<sf::SoundBuffer> buffers_som_sfml;

shared_ptr<sf::SoundBuffer> carregar_audio_buffer_sfml(string local)
{
	if (buffers_som_sfml.pegar(local) == NULL)
	{
		buffers_som_sfml.aplicar(local, sf::SoundBuffer());
		buffers_som_sfml.pegar(local)->loadFromFile(local);
	}
	return buffers_som_sfml.pegar(local);
}

void carregar_audio_buffer_thread(string local, shared_ptr<sf::SoundBuffer> *ret)
{
	*ret = carregar_audio_buffer_sfml(local);
}

shared_ptr<transform_> listener_transform = NULL;

void set_global_volume_sfml(float vol){sf::Listener::setGlobalVolume(vol);}

class sfml_audio : public componente
{
public:
	// https://www.sfml-dev.org/tutorials/2.5/audio-spatialization.php

	audio_info info;
	sf::Sound som;
	shared_ptr<sf::SoundBuffer> buffer;

	shared_ptr<transform_> tf = NULL;

	void aplicar_info()
	{

		buffer = carregar_audio_buffer_sfml(info.nome);
		som.setBuffer(*buffer.get());

		som.setPlayingOffset(sf::seconds(info.tempo));
		som.setVolume(info.volume);
		som.setLoop(info.loop);
		som.setPitch(info.velocidade);
		som.setMinDistance(info.min_distance);
		som.setAttenuation(info.atenuation);
		if (info.pausa)
		{
			som.pause();
		}
		else
		{
			som.play();
		}
		tf = esse_objeto->pegar_componente<transform_>();
	}

	audio_info pegar_info()
	{
		audio_info ret;

		ret.tempo = som.getPlayingOffset().asSeconds();
		ret.volume = som.getVolume();
		ret.loop = som.getLoop();
		ret.velocidade = som.getPitch();
		ret.min_distance = som.getMinDistance();
		ret.atenuation = som.getAttenuation();

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
			vec3 ldir = listener_transform->pegar_direcao_local(vec3(0, 0, -1));

			sf::Listener::setPosition(lpos.x, lpos.y, lpos.z);
			sf::Listener::setUpVector(lup.x, lup.y, lup.z);
			sf::Listener::setDirection(ldir.x, ldir.y, ldir.z);

			vec3 gpos = tf->pegar_pos_global();
			som.setPosition(-gpos.x, gpos.y, -gpos.z);

		}
		else
		{
			sf::Listener::setPosition(0,0,0);
			sf::Listener::setUpVector(0,1,0);
			sf::Listener::setDirection(0,0,-1);
			som.setPosition(0,0,0);
		}
	}

	sfml_audio()
	{
	}
	sfml_audio(audio_info info)
	{
		this->info = info;
	}

	void finalisar()
	{
		som.pause();
	}
};
