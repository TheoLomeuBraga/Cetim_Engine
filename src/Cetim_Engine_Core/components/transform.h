#pragma once

#include <iostream>
#include <functional>
using String = std::string;
#include "RecursosT.h"
#include "game_object.h"

mat4 MatrizMundi = glm::mat4(1.0);

class transform_ : public componente
{
public:
	glm::mat4 matrizTransform;
	glm::mat4 offset_matrix = glm::mat4(1.0f);
	unsigned char billboarding = 0;
	bool UI = false;
	transform_ *paiTF = NULL;

	int local_hierarquia;

	bool usar_pai = true;
	mat4 pai_matriz;

	glm::vec3 pos = vec3(0, 0, 0), esca = vec3(1, 1, 1), rot = vec3(0, 0, 0);

	quat quater = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	transform_()
	{
		matrizTransform = MatrizMundi;
	}
	~transform_()
	{
		matrizTransform = MatrizMundi;
	}

	glm::mat4 pegar_matriz()
	{
		glm::mat4 ret;

		if (paiTF == NULL || !usar_pai)
		{
			ret = MatrizMundi;
		}
		else
		{
			ret = paiTF->matrizTransform;
		}

		ret = translate(ret, pos);

		ret *= toMat4(quater);
		ret = scale(ret, esca);

		// matrizTransform = ret;

		return ret;
	}

	glm::mat4 pegar_matriz_local()
	{
		glm::mat4 ret;

		ret = MatrizMundi;

		ret = translate(ret, pos);

		ret *= toMat4(quater);
		ret = scale(ret, esca);

		// matrizTransform = ret;

		return ret;
	}

	glm::vec3 pegar_pos_global()
	{
		return glm::vec3(pegar_matriz()[3]);
	}
	glm::quat pegar_qua_global()
	{
		return glm::quat(glm::mat3(pegar_matriz()));
	}
	glm::vec3 pegar_graus_global()
	{
		return quat_graus(pegar_qua_global());
	}
	glm::vec3 pegar_scale_global()
	{
		glm::vec3 scale;
		glm::mat4 matrix = pegar_matriz();
		scale.x = glm::length(glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]));
		scale.y = glm::length(glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]));
		scale.z = glm::length(glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]));

		return scale;
	}

	vec3 pegar_direcao_local(vec3 dir)
	{
		glm::mat4 translationMatrix = glm::translate(matrizTransform, dir);
		glm::vec3 ret = glm::vec3(translationMatrix[3]) - pegar_pos_global();
		return glm::vec3(ret.x, ret.y, ret.z);
	}

	vec3 pegar_direcao_global(vec3 dir)
	{
		glm::mat4 translationMatrix = glm::translate(matrizTransform, dir);
		glm::vec3 ret = glm::vec3(translationMatrix[3]);
		return glm::vec3(ret.x, ret.y, ret.z);
	}

	vec3 mover_direcao(vec3 dir)
	{
		vec3 nada;
		vec4 nada2;
		vec3 escala;
		vec3 pos;
		quat qua;
		glm::decompose(glm::translate(matrizTransform, dir), escala, qua, pos, nada, nada2);

		return pos;
	}

	void reset(bool ui, transform_ *pai, glm::vec3 p, glm::vec3 r, glm::vec3 e)
	{
		UI = ui;
		paiTF = pai;
		pos = p;
		quater = graus_quat(r);
		esca = e;
		pegar_matriz();
	}

	glm::vec3 billboarding_spherical(glm::vec3 target, glm::vec3 up = vec3(0, 1, 0))
	{
		glm::vec3 position = pegar_matriz()[3];
		up = pegar_direcao_local(up);

		glm::vec3 direction = glm::normalize(target - position);

		// Calcula os ângulos de pitch e yaw
		float pitch = glm::degrees(asin(-direction.y));
		float yaw = glm::degrees(atan2(direction.x, direction.z));

		// Constrói o vetor em graus
		glm::vec3 euler_angles = glm::vec3(pitch, yaw, 0.0f);

		euler_angles.z = up.z;

		quater = graus_quat(euler_angles);
		return euler_angles;
	}

	glm::vec3 billboarding_planar(glm::vec3 target, glm::vec3 up = vec3(0, 1, 0))
	{
		glm::vec3 position = pegar_matriz()[3];
		target.y = position.y;
		up = pegar_direcao_local(up);

		glm::vec3 direction = glm::normalize(target - position);

		// Calcula os ângulos de pitch e yaw
		float pitch = glm::degrees(asin(-direction.y));
		float yaw = glm::degrees(atan2(direction.x, direction.z));

		// Constrói o vetor em graus
		glm::vec3 euler_angles = glm::vec3(pitch, yaw, 0.0f);

		quater = graus_quat(euler_angles);
		return euler_angles;
	}

	glm::vec3 olhar_para(glm::vec3 up_axe, glm::vec3 look_axe, glm::vec3 target, bool usar_eixo_y = false)
	{
		glm::vec3 start = vec3(pegar_matriz()[3]);
		if (!usar_eixo_y)
		{
			start.y = target.y;
		}

		glm::vec3 forward = glm::normalize(target - start);

		// Calcula o vetor de rotação entre o eixo de visualização atual e o novo
		glm::vec3 axis = glm::cross(look_axe, forward);

		// Calcula o ângulo entre o eixo de visualização atual e o novo
		float angle = acos(glm::dot(glm::normalize(look_axe), glm::normalize(forward)));

		// Cria a matriz de rotação
		glm::mat4 rotationMatrix = glm::toMat4(glm::angleAxis(angle, axis));

		// Aplica a rotação ao eixo de "frente" para obter o novo vetor de direção
		glm::vec4 new_forward = rotationMatrix * glm::vec4(look_axe, 0.0f);

		glm::vec3 degres = glm::degrees(glm::vec3(new_forward));

		print("degres",degres.x,degres.y,degres.z);

		quater = graus_quat(degres);

		print("quater");

		// Retorna a direção olhando para o alvo com o novo eixo de "cima"
		return degres;
	}

	void atualizar_tf()
	{
		matrizTransform = pegar_matriz();
	}

	transform_(glm::vec3 p, glm::vec3 r, glm::vec3 e)
	{
		reset(false, NULL, p, r, e);
	}

	transform_(bool ui, glm::vec3 p, glm::vec3 r, glm::vec3 e)
	{
		reset(ui, NULL, p, r, e);
	}

	transform_(transform_ *pai, glm::vec3 p, glm::vec3 r, glm::vec3 e)
	{
		reset(false, pai, p, r, e);
	}

	transform_(bool ui, transform_ *pai, glm::vec3 p, glm::vec3 r, glm::vec3 e)
	{
		reset(ui, pai, p, r, e);
	}

	vec3 pegar_angulo_graus()
	{
		rot = quat_graus(quater);
		return rot;
	}
	void mudar_angulo_graus(vec3 ang)
	{
		rot = ang;
		quater = graus_quat(ang);
	}
};

float tf_distance(glm::vec3 a, shared_ptr<transform_> tf)
{
	return glm::length(a - vec3(tf->matrizTransform[3]));
}

// Function to sort the input vector of vec3 points based on the distance from the given point
std::vector<shared_ptr<objeto_jogo>> tf_ordenate_by_distance(glm::vec3 point, std::vector<shared_ptr<objeto_jogo>> objs)
{
	// Create a vector to store the distances
	std::vector<float> tf_distance(objs.size());

	// Calculate the distances from the given point to each of the points in the input vector
	for (int i = 0; i < objs.size(); i++)
	{
		if (objs[i]->pegar_componente<transform_>() != NULL)
		{
			tf_distance[i] = vec3_distance(point, vec3(objs[i]->pegar_componente<transform_>()->matrizTransform[3]));
		}
	}

	// Create an index vector and sort it based on the distances
	std::vector<int> indices(objs.size());
	std::iota(indices.begin(), indices.end(), 0);
	std::sort(indices.begin(), indices.end(), [&](int i, int j)
			  { return tf_distance[i] < tf_distance[j]; });

	// Create a new vector to store the sorted vec3 points
	std::vector<shared_ptr<objeto_jogo>> ret(objs.size());

	// Fill the new vector with the vec3 points in the order determined by the sorted index vector
	for (int i = 0; i < indices.size(); i++)
	{
		ret[i] = objs[indices[i]];
	}

	// Return the sorted vector of vec3 points
	return ret;
}

std::vector<shared_ptr<objeto_jogo>> tf_ordenate_by_distance_far(glm::vec3 point, std::vector<shared_ptr<objeto_jogo>> objs)
{
	std::vector<shared_ptr<objeto_jogo>> ret = tf_ordenate_by_distance(point, objs);
	std::reverse(ret.begin(), ret.end());
	return ret;
}

#include "ecs/ecs.h"
#include "ecs/ecs_components_systems/ecs_parents.h"
#include <set>

namespace transform_ecs
{

	struct transform_data
	{
		mat4 matrix;
		vec3 position;
		quat rotation;
		vec3 scale;
	};

	std::map<entity, struct transform_data> transforms_map;

	entity main_transform_entity = 0;

	void add_transform(entity id, std::any data)
	{
		transforms_map.insert(std::pair<entity, struct transform_data>(id, std::any_cast<struct transform_data>(data)));
	}

	bool have_transform(entity id)
	{
		if (transforms_map.find(id) != transforms_map.end())
		{
			return true;
		}
		return false;
	}

	void run_transform(entity id) {}

	void run_transforms()
	{
		for (pair<entity, struct transform_data> p : transforms_map)
		{
			glm::mat4 ret;

			transform_data *father = NULL;
			transform_data *tf = &transforms_map[p.first];

			if (have_component(p.first, "family"))
			{
				ret = father->matrix;
			}
			else
			{
				ret = glm::mat4(1.0f);
			}

			ret = translate(ret, tf->position);

			ret *= toMat4(tf->rotation);
			ret = scale(ret, tf->scale);

			tf->matrix = ret;
		}
	}

	void remove_transform(entity id)
	{
		transforms_map.erase(id);
	}

	void register_transform_component()
	{
		ecs_systems_registerd.insert(std::pair<std::string, struct ecs_system>("transform", {add_transform, have_transform, run_transform, run_transforms, remove_transform}));
	}

	// transform_ecs functions

	glm::vec3 pegar_pos_global(entity id)
	{
		transform_data *tf = &transforms_map[id];

		return glm::vec3(tf->matrix[3]);
	}
	glm::quat pegar_qua_global(entity id)
	{
		transform_data *tf = &transforms_map[id];

		vec3 nada;
		vec4 nada2;
		vec3 nada3;
		quat qua;
		glm::decompose(tf->matrix, nada, qua, nada3, nada, nada2);
		return qua;
	}
	glm::vec3 pegar_graus_global(entity id)
	{
		transform_data *tf = &transforms_map[id];

		vec3 nada;
		vec4 nada2;
		vec3 nada3;
		quat qua;
		glm::decompose(tf->matrix, nada, qua, nada3, nada, nada2);

		return quat_graus(qua);
	}

	vec3 pegar_direcao_local(entity id, vec3 dir)
	{
		transform_data *tf = &transforms_map[id];

		glm::mat4 translationMatrix = glm::translate(tf->matrix, dir);
		glm::vec3 ret = glm::vec3(translationMatrix[3]) - glm::vec3(tf->matrix[3]);
		return glm::vec3(ret.x, ret.y, ret.z);
	}

	vec3 pegar_direcao_global(entity id, vec3 dir)
	{
		transform_data *tf = &transforms_map[id];

		glm::mat4 translationMatrix = glm::translate(tf->matrix, dir);
		glm::vec3 ret = glm::vec3(translationMatrix[3]);
		return glm::vec3(ret.x, ret.y, ret.z);
	}

	vec3 mover_direcao(entity id, vec3 dir)
	{
		transform_data *tf = &transforms_map[id];

		vec3 nada;
		vec4 nada2;
		vec3 escala;
		vec3 pos;
		quat qua;
		glm::decompose(glm::translate(tf->matrix, dir), escala, qua, pos, nada, nada2);

		return pos;
	}
};
