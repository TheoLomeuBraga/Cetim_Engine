#pragma once

#include <iostream>
#include <functional>
using namespace std;
#include "RecursosT.h"
#include "game_object.h"

mat4 MatrizMundi = glm::mat4(1.0);

class transform_ : public componente
{
public:
	glm::mat4 matrizTransform;

	transform_()
	{
		matrizTransform = MatrizMundi;
	}
	~transform_()
	{
		matrizTransform = MatrizMundi;
	}

	bool UI = false;
	transform_ *paiTF = NULL;

	int local_hierarquia;

	bool usar_pai_matriz;
	mat4 pai_matriz;

	glm::vec3 pos = vec3(0, 0, 0), esca = vec3(1, 1, 1), rot = vec3(0, 0, 0);

	quat quater;

	glm::mat4 pegar_matriz()
	{
		glm::mat4 ret;

		if (paiTF == NULL)
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

		matrizTransform = ret;

		return ret;
	}

	glm::vec3 pegar_pos_global()
	{
		return glm::vec3(pegar_matriz()[3]); //
	}
	glm::quat pegar_qua_global()
	{
		vec3 nada;
		vec4 nada2;
		vec3 nada3;
		quat qua;
		glm::decompose(matrizTransform, nada, qua, nada3, nada, nada2);
		return qua;
	}
	glm::vec3 pegar_graus_global()
	{
		return quat_graus(pegar_qua_global());
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

	void atualizar_tf()
	{
		pegar_matriz();
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
	return glm::length(a - tf->pos);
}

// Function to sort the input vector of vec3 points based on the distance from the given point
std::vector<shared_ptr<objeto_jogo>> tf_ordenate_by_distance(glm::vec3 point, std::vector<shared_ptr<objeto_jogo>> objs)
{
	// Create a vector to store the distances
	std::vector<float> tf_distance(objs.size());

	// Calculate the distances from the given point to each of the points in the input vector
	for (int i = 0; i < objs.size(); i++)
	{
		tf_distance[i] = vec3_distance(point, objs[i]->pegar_componente<transform_>()->pos);
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

			transform_data *father =  NULL;
			transform_data *tf =  &transforms_map[p.first];

			if (have_component(p.first,"family"))
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
		transform_data *tf =  &transforms_map[id];
		
		return glm::vec3(tf->matrix[3]);
	}
	glm::quat pegar_qua_global(entity id)
	{
		transform_data *tf =  &transforms_map[id];

		vec3 nada;
		vec4 nada2;
		vec3 nada3;
		quat qua;
		glm::decompose(tf->matrix, nada, qua, nada3, nada, nada2);
		return qua;
	}
	glm::vec3 pegar_graus_global(entity id)
	{
		transform_data *tf =  &transforms_map[id];

		vec3 nada;
		vec4 nada2;
		vec3 nada3;
		quat qua;
		glm::decompose(tf->matrix, nada, qua, nada3, nada, nada2);

		return quat_graus(qua);
	}

	vec3 pegar_direcao_local(entity id,vec3 dir)
	{
		transform_data *tf =  &transforms_map[id];

		glm::mat4 translationMatrix = glm::translate(tf->matrix, dir);
		glm::vec3 ret = glm::vec3(translationMatrix[3]) - glm::vec3(tf->matrix[3]);
		return glm::vec3(ret.x, ret.y, ret.z);
	}

	vec3 pegar_direcao_global(entity id,vec3 dir)
	{
		transform_data *tf =  &transforms_map[id];

		glm::mat4 translationMatrix = glm::translate(tf->matrix, dir);
		glm::vec3 ret = glm::vec3(translationMatrix[3]);
		return glm::vec3(ret.x, ret.y, ret.z);
	}

	vec3 mover_direcao(entity id,vec3 dir)
	{
		transform_data *tf =  &transforms_map[id];

		vec3 nada;
		vec4 nada2;
		vec3 escala;
		vec3 pos;
		quat qua;
		glm::decompose(glm::translate(tf->matrix, dir), escala, qua, pos, nada, nada2);

		return pos;
	}
};
