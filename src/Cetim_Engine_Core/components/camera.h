#pragma once

#include <iostream>
#include <functional>
using String = std::string;
#include "transform.h"
#include "RecursosT.h"
#include "game_object.h"

#include <glm/gtc/matrix_inverse.hpp>

glm::mat4 getCameraViewMatrix(glm::mat4 transformMatrix)
{
	// Remove a informação de escala da matriz de transformação
	glm::mat4 viewMatrix = glm::mat4(glm::mat3(transformMatrix));

	// Remove a informação de transladação da matriz de transformação
	viewMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Inverte a matriz resultante
	viewMatrix = glm::affineInverse(viewMatrix);

	return viewMatrix;
}

class camera : public componente
{
private:
public:
	shared_ptr<transform_> paiTF;

	glm::vec3 pos, alvo, cima;

	bool ortografica = false;
	float zoom = 90;
	vec2 tamanho = vec2(20, 20), res = vec2(1, 1);
	float ncp = 0.01;
	float fcp = 100;
	glm::mat4 matrizVisao, matrizProjecao;

	int8_t prioridade = 0;

	imagem *alvo_render;

	camera() {}

	void pegar_matriz_visao()
	{
		if (paiTF != NULL)
		{

			mat4 new_mat;
			vec3 nada;
			vec4 nada2;
			vec3 pos;
			quat qua;

			if (paiTF->paiTF == NULL)
			{
				new_mat = paiTF->matrizTransform;
			}
			else
			{
				new_mat = paiTF->paiTF->matrizTransform;
			}

			new_mat = translate(new_mat, paiTF->pos);

			vec3 rot = quat_graus(paiTF->quater);
			rot = vec3(-rot.x, rot.y, -rot.z);

			new_mat *= toMat4(graus_quat(rot));

			glm::decompose(new_mat, nada, qua, pos, nada, nada2);

			matrizVisao = getCameraViewMatrix(new_mat);
			matrizVisao = translate(matrizVisao, vec3(pos.x, -pos.y, pos.z));
		}
	}

	void atualizar_tf()
	{

		pegar_matriz_visao();
	}

	glm::mat4 gerar_matriz_perspectiva(float zoom, int resX, int resY, float ncp, float fcp)
	{

		mat4 ret = glm::perspective(glm::radians(zoom), (float)(resX / resY), ncp, fcp);
		matrizProjecao = ret;
		res.x = resX;
		res.y = resY;
		this->fcp = fcp;
		this->ncp = ncp;
		return ret;
	}

	glm::mat4 gerar_matriz_ortografica(float tamanhoX, float tamanhoY, float ncp, float fcp)
	{

		mat4 ret = glm::ortho(-tamanhoX / 2, tamanhoX / 2, -tamanhoY / 2, tamanhoY / 2, ncp, fcp);
		matrizProjecao = ret;
		this->fcp = fcp;
		this->ncp = ncp;
		return ret;
	}

	void configurar_camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float ZooM, int resX, int resY, float ncp, float fcp)
	{
		pos = p;
		alvo = a;
		cima = c;

		// atualizar_tf();

		ortografica = false;
		zoom = ZooM;
		this->fcp = fcp;
		this->ncp = ncp;
		matrizProjecao = gerar_matriz_perspectiva(ZooM, resX, resY, ncp, fcp);

		atualizar_tf();
	}

	void configurar_camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float tamanhoX, float tamanhoY, float ncp, float fcp)
	{
		pos = p;
		alvo = a;
		cima = c;

		// atualizar_tf();

		ortografica = true;
		tamanho = vec2(tamanhoX, tamanhoY);
		this->fcp = fcp;
		this->ncp = ncp;
		matrizProjecao = gerar_matriz_ortografica(tamanhoX, tamanhoY, ncp, fcp);

		atualizar_tf();
	}

	camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float zoom, int resX, int resY, float ncp, float fcp)
	{
		configurar_camera(p, a, c, zoom, resX, resY, ncp, fcp);
	}

	camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float tamanhoX, float tamanhoY, float ncp, float fcp)
	{
		configurar_camera(p, a, c, tamanhoX, tamanhoY, ncp, fcp);
	}
};

#include "ecs/ecs.h"
#include <vector>
#include <string>
#include <map>
#include <set>

namespace camera_ecs
{

	struct camera_data
	{
		mat4 view;
		mat4 projection;
		bool orto = false;
		float ncp = 0.01;
		float fcp = 100.0;
		float zoom = 90;
		vec2 size = vec2(20, 20);
		vec2 resolution = vec2(1, 1);
	};

	std::map<entity, camera_data> cameras_map;

	camera_data create_perspective_camera(float zoom, int resX, int resY, float ncp, float fcp)
	{
		camera_data ret;
		ret.projection = glm::perspective(glm::radians(zoom), (float)(resX / resY), ncp, fcp);
		ret.fcp = fcp;
		ret.ncp = ncp;
		ret.resolution = vec2(resX, resY);

		ret.orto = false;
		ret.zoom = zoom;
		return ret;
	}

	camera_data create_orto_camera(float tamanhoX, float tamanhoY, float ncp, float fcp)
	{
		camera_data ret;
		ret.projection = glm::ortho(-tamanhoX / 2, tamanhoX / 2, -tamanhoY / 2, tamanhoY / 2, ncp, fcp);
		ret.fcp = fcp;
		ret.ncp = ncp;
		ret.orto = true;

		ret.size = vec2(tamanhoX, tamanhoY);

		return ret;
	}

	void add_camera(entity id, std::any data)
	{
		cameras_map.insert(std::pair<entity, camera_data>(id, std::any_cast<camera_data>(data)));
	}

	bool have_camera(entity id)
	{
		if (cameras_map.find(id) != cameras_map.end())
		{
			return true;
		}
		return false;
	}

	void run_camera(entity id)
	{

		camera_data *cam = &cameras_map[id];

		if (!cam->orto)
		{
			*cam = create_perspective_camera(cam->zoom, cam->resolution.x, cam->resolution.y, cam->ncp, cam->fcp);
		}
		else
		{
			*cam = create_orto_camera(cam->size.x, cam->size.y, cam->ncp, cam->fcp);
		}

		if (have_component(id, "transform"))
		{
			transform_ecs::transform_data *tf = &transform_ecs::transforms_map[id]; // end this

			mat4 new_mat = tf->matrix;
			vec3 nada;
			vec4 nada2;
			vec3 pos;
			quat qua;

			//terminar isso

			vec3 rot = quat_graus(tf->rotation);
			rot = vec3(-rot.x, rot.y, -rot.z);
			new_mat *= toMat4(graus_quat(rot));
			glm::decompose(new_mat, nada, qua, pos, nada, nada2);

			cam->view = getCameraViewMatrix(new_mat);
			cam->view = translate(cam->view, vec3(pos.x, -pos.y, pos.z));
			
		}
	}

	void run_cameras()
	{
		for (pair<entity, camera_data> p : cameras_map)
		{
			run_camera(p.first);
		}
	}

	void remove_camera(entity id)
	{
		cameras_map.erase(id);
	}

	void register_camera_component()
	{
		ecs_systems_registerd.insert(std::pair<std::string, struct ecs_system>("camera", {add_camera, have_camera, run_camera, run_cameras, remove_camera}));
	}

	// terminar criar funções camera

};
