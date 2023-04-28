#pragma once
#include <iostream>
#include <memory>
#include <SFML/Audio.hpp>
#include "Config.h"
#include "RecursosT.h"
#include "scene.h"
#include "game_object.h"
#include "components/transform.h"

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

class sfml_audio : public componente
{
public:
	// https://www.sfml-dev.org/tutorials/2.5/audio-spatialization.php

	audio_info info;
	sf::Sound som;
	shared_ptr<sf::SoundBuffer> buffer;
	float min_distance = 0, atenuation = 0;

	shared_ptr<transform_> tf = NULL;

	void aplicar_info()
	{

		buffer = carregar_audio_buffer_sfml(info.nome);
		som.setBuffer(*buffer.get());

		som.setPlayingOffset(sf::seconds(info.tempo));
		som.setVolume(info.volume);
		som.setLoop(info.loop);
		som.setPitch(info.velocidade);
		if (info.pausa)
		{
			som.pause();
		}
		else
		{
			som.play();
		}
		som.setMinDistance(info.min_distance);
		som.setAttenuation(info.atenuation);
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

		if (listener_transform != NULL && tf != NULL)
		{
			vec3 lpos = listener_transform->pegar_pos_global(), ldir;
			sf::Listener::setPosition(lpos.x, lpos.y, lpos.z);
			ldir = listener_transform->pegar_direcao_local(vec3(1, 0, 0));
			sf::Listener::setUpVector(ldir.x, ldir.y, ldir.z);

			vec3 gpos = tf->pegar_pos_global();
			som.setPosition(gpos.x, gpos.y, gpos.z);

			sf::Listener::setPosition(0, 0, 0);
			sf::Listener::setUpVector(0, 0, 0);
		}
		else
		{
			sf::Listener::setPosition(0, 0, 0);
			sf::Listener::setUpVector(1, 0, 0);
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
