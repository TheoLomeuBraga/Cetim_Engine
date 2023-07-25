#pragma once
#include <set>
#include "RecursosT.h"
#include "scene.h"
#include "Tempo.h"

#include "box2d/box2d.h"
#include "game_object.h"

int32 velocidade_interacoes = 6;
int32 iteracao_posicao = 2;
float passo_tempo;
float ultimo_tempo;

static b2Vec2 converter_b2D(vec3 v)
{
	return b2Vec2(v.x, v.y);
}
static b2Vec2 converter_b2D(vec2 v)
{
	return b2Vec2(v.x, v.y);
}
vector<b2Vec2> converter_b2D(vector<vec2> vs)
{
	
	vector<b2Vec2> ret;
	//ret.resize(vs.size());

	for (int i = 0; i < vs.size(); i++)
	{
		ret.push_back(converter_b2D(vs[i]));
		//print({"b2Vec2",vs[i].x,vs[i].y});
	}

	return ret;
}
vector<b2Vec2> converter_b2D(vector<vec2> vs, vec2 escala)
{
	vector<b2Vec2> ret;
	//ret.resize(vs.size());

	for (int i = 0; i < vs.size(); i++)
	{
		b2Vec2 b2v = b2Vec2(vs[i].x * escala.x, vs[i].y * escala.y);
		ret.push_back(b2v);
		//print({"b2Vec2",ret[i].x,ret[i].y});
	}

	return ret;
}

b2World mundo(converter_b2D(gravidade));

map<b2Body *, shared_ptr<objeto_jogo>> corpo_obj;

b2Vec2 tile_vertex[] = {
	b2Vec2(0, 0.51),
	b2Vec2(0.5, 0.5),
	b2Vec2(0.51, 0),
	b2Vec2(0.5, -0.5),
	b2Vec2(0, -0.51),
	b2Vec2(-0.5, -0.5),
	b2Vec2(0, 0),
	b2Vec2(-0.5, 0.5),
};
b2Vec2 tile_vertex2[] = {
	b2Vec2(0, 0.51),
	b2Vec2(0.5, 0.5),
	b2Vec2(0.51, 0),
	b2Vec2(0.5, -0.5),
	b2Vec2(0, -0.51),
	b2Vec2(-0.5, -0.5),
	b2Vec2(0, 0),
	b2Vec2(-0.5, 0.5),
};

b2Vec2 tiled_volume[] = {
	b2Vec2(1, 0),
	b2Vec2(1, -1),
	b2Vec2(0, 0),
	b2Vec2(0, -1)};

b2Vec2 tiled_volume2[] = {
	b2Vec2(1, 0),
	b2Vec2(1, -1),
	b2Vec2(0, 0),
	b2Vec2(0, -1)};

class raycast_retorno : public b2RayCastCallback
{
public:
	raycast_retorno() {}

	colis_info ci;

	float ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction)
	{
		ci.cos_obj = corpo_obj[fixture->GetBody()].get();
		ci.pos = vec3(point.x, point.y, 0);
		ci.nor = vec3(normal.x, normal.y, 0);
		return 0;
	};
};

bool box_2D_iniciado;
class box_2D : public componente
{
	vector<b2Vec2> vertices_B2;
	bool iniciado = false;

public:
	b2Body *corpo = NULL;

	char forma = caixa;
	b2CircleShape Circle;
	b2PolygonShape Vertex_shape;
	float densidade = 1, atrito = 0;
	bool gatilho = false;
	char dinamica = estatico;
	b2BodyDef BodyDef;
	b2FixtureDef fixtureDef;
	vec2 escala = vec2(1, 1);
	float escala_gravidade = 1;
	bool rotacionar = true;
	vector<vec2> vertices;

	info_camada camada;
	vector<colis_info> colis_infos;
	vector<shared_ptr<objeto_jogo>> objs_colidindo;

	box_2D() {}

	void mudar_pos(vec2 pos)
	{
		
		corpo->SetTransform(b2Vec2(pos.x, pos.y), corpo->GetAngle());
	}

	void mudar_rot(float angulo)
	{
		corpo->SetTransform(corpo->GetPosition(), angulo);
	}

	void iniciar()
	{
		iniciado = true;

		// BodyDef
		if (dinamica == estatico)
		{
			BodyDef.type = b2_staticBody;
		}
		else if (dinamica == cinematico)
		{
			BodyDef.type = b2_kinematicBody;
		}
		else if (dinamica == dinamico)
		{
			BodyDef.type = b2_dynamicBody;
		}
		if (esse_objeto->pegar_componente<transform_>() != NULL)
		{
			BodyDef.position = converter_b2D(esse_objeto->pegar_componente<transform_>()->pegar_pos_global());
			vec3 rot = glm::radians(quat_graus(esse_objeto->pegar_componente<transform_>()->pegar_graus_global()));
			BodyDef.angle = rot.z;
		}
		BodyDef.fixedRotation = !rotacionar;

		// b2FixtureDef
		if (forma == caixa)
		{
			Vertex_shape.SetAsBox(escala.x, escala.y);
			fixtureDef.shape = &Vertex_shape;
		}
		else if (forma == esfera)
		{
			Circle.m_p.Set(0.0f, 0.0f);
			Circle.m_radius = escala.x;
			fixtureDef.shape = &Circle;
		}
		else if (forma == convexo)
		{
			if (esse_objeto->pegar_componente<transform_>() != NULL)
			{
				vertices_B2 = converter_b2D(vertices, escala);
				Vertex_shape.Set(&vertices_B2[0], 8);
			}
			else
			{
				vertices_B2 = converter_b2D(vertices);
				Vertex_shape.Set(&vertices_B2[0], vertices_B2.size());
			}
			fixtureDef.shape = &Vertex_shape;
		}
		else if (forma == tile_suave)
		{
			for (int i = 0; i < 8; i++)
			{
				tile_vertex2[i] = b2Vec2(tile_vertex[i].x * escala.x, tile_vertex[i].y * escala.y);
			}
			Vertex_shape.Set(&tile_vertex2[0], 8);
			fixtureDef.shape = &Vertex_shape;
		}
		else if (forma == volume_tiled)
		{

			for (int i = 0; i < 4; i++)
			{
				tiled_volume2[i] = b2Vec2(tiled_volume[i].x * escala.x, tiled_volume[i].y * escala.y);
			}
			Vertex_shape.Set(&tiled_volume2[0], 4);
			fixtureDef.shape = &Vertex_shape;
		}

		fixtureDef.density = densidade;
		fixtureDef.friction = atrito;
		fixtureDef.isSensor = gatilho;

		// corpo

		corpo = mundo.CreateBody(&BodyDef);
		corpo->CreateFixture(&fixtureDef);

		corpo_obj.insert(pair<b2Body *, shared_ptr<objeto_jogo>>(corpo, esse_objeto));

		if (esse_objeto->pegar_componente<transform_>() != NULL)
		{
			//vec3 pos = esse_objeto->pegar_componente<transform_>()->pegar_pos_global();
			//float rot = esse_objeto->pegar_componente<transform_>()->pegar_graus_global().z;
			vec3 pos = esse_objeto->pegar_componente<transform_>()->pos;
			float rot = quat_graus(esse_objeto->pegar_componente<transform_>()->quater).z;
			mudar_pos(vec2(pos.x, pos.y));
			mudar_rot(rot);
		}
	}

	void atualisar()
	{
		for (colis_info c : colis_infos)
		{
			esse_objeto->colidir(c);
		}
		vector<colis_info> vazio;
		colis_infos.swap(vazio);

		corpo->SetGravityScale(escala_gravidade);
		shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
		if (tf != NULL)
		{
			b2Vec2 pos = corpo->GetPosition();
			tf->pos = vec3(pos.x, pos.y, esse_objeto->pegar_componente<transform_>()->pos.z);
			vec3 rot = tf->pegar_angulo_graus();
			tf->quater = quat(vec3(rot.x, rot.y, corpo->GetAngle()));
		}
	}

	void finalisar()
	{
		// AAAAA
		iniciado = false;
		if (corpo != NULL)
		{
			corpo->GetWorld()->DestroyBody(corpo);
			corpo_obj.erase(corpo);
		}

		corpo = NULL;
	}
	~box_2D()
	{
		vector<colis_info> vazioA;
		colis_infos.swap(vazioA);

		vector<shared_ptr<objeto_jogo>> vazioB;
		objs_colidindo.swap(vazioB);
	}
	void aplicar()
	{
		if (corpo != NULL)
		{
			finalisar();
			iniciar();
		}
	}

	

	void mover(vec2 forca)
	{
		corpo->SetLinearVelocity(b2Vec2(forca.x, forca.y));
	}

	void adicionar_forca(vec2 forca)
	{
		corpo->ApplyForce(b2Vec2(forca.x, forca.y), corpo->GetWorldCenter(), true);
	}
	void adicionar_impulso(vec2 forca)
	{
		corpo->ApplyLinearImpulse(b2Vec2(forca.x, forca.y), corpo->GetWorldCenter(), true);
	}
	
	void adicionar_forca_rotativo(float forca){
		corpo->ApplyTorque(forca,true);
	}
	void adicionar_impulso_rotativo(float forca){
		corpo->ApplyAngularImpulse(forca,true);
	}


	// https://www.iforce2d.net/b2dtut/raycasting
	static bool ray_cast(vec2 pos, float distancia, float angulo)
	{
		bool ret = false;

		raycast_retorno cb;
		mundo.RayCast(&cb, b2Vec2(pos.x, pos.y), b2Vec2(pos.x, pos.y) + distancia * b2Vec2(sinf(angulo), cosf(angulo)));
		if (cb.ci.cos_obj != NULL)
		{
			ret = true;
		}
		return ret;
	}

	static bool ray_cast(vec2 pos, float distancia, float angulo, colis_info &colis)
	{
		bool ret = false;

		raycast_retorno cb;
		mundo.RayCast(&cb, b2Vec2(pos.x, pos.y), b2Vec2(pos.x, pos.y) + distancia * b2Vec2(sinf(angulo), cosf(angulo)));

		colis = cb.ci;

		if (cb.ci.cos_obj != NULL)
		{
			ret = true;
		}
		return ret;
	}
};

class filtro_colisao : public b2ContactFilter
{
public:
	filtro_colisao(){};

	bool ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB)
	{

		bool ret = false;
		b2Body *corpoA = fixtureA->GetBody();
		b2Body *corpoB = fixtureB->GetBody();
		for (int i : corpo_obj[corpoA]->pegar_componente<box_2D>()->camada.camada_colide)
		{
			if (i == corpo_obj[corpoB]->pegar_componente<box_2D>()->camada.camada)
			{
				ret = true;
				break;
			}
		}

		return ret;
	};
};

class m_contactlistener : public b2ContactListener
{
public:
	void BeginContact(b2Contact *contact)
	{
		b2Body *corpoA = contact->GetFixtureA()->GetBody();
		b2Body *corpoB = contact->GetFixtureB()->GetBody();
		corpo_obj[corpoA]->pegar_componente<box_2D>()->objs_colidindo.push_back(corpo_obj[corpoB]);
		corpo_obj[corpoB]->pegar_componente<box_2D>()->objs_colidindo.push_back(corpo_obj[corpoA]);

		colis_info ci;

		ci.velocidade = contact->GetTangentSpeed();

		ci.colidindo = contact->IsTouching();

		ci.obj = corpo_obj[corpoA].get();
		ci.cos_obj = corpo_obj[corpoB].get();
		corpo_obj[corpoA]->pegar_componente<box_2D>()->colis_infos.push_back(ci);

		ci.obj = corpo_obj[corpoB].get();
		ci.cos_obj = corpo_obj[corpoA].get();
		corpo_obj[corpoB]->pegar_componente<box_2D>()->colis_infos.push_back(ci);
	}

	void EndContact(b2Contact *contact)
	{
		b2Body *corpoA = contact->GetFixtureA()->GetBody();
		b2Body *corpoB = contact->GetFixtureB()->GetBody();

		for (int i = 0; i < corpo_obj[corpoA]->pegar_componente<box_2D>()->objs_colidindo.size(); i++)
		{
			if (corpo_obj[corpoA]->pegar_componente<box_2D>()->objs_colidindo[i] == corpo_obj[corpoB])
			{
				corpo_obj[corpoA]->pegar_componente<box_2D>()->objs_colidindo.erase(corpo_obj[corpoA]->pegar_componente<box_2D>()->objs_colidindo.begin() + i);
				break;
			}
		}
		for (int i = 0; i < corpo_obj[corpoB]->pegar_componente<box_2D>()->objs_colidindo.size(); i++)
		{
			if (corpo_obj[corpoB]->pegar_componente<box_2D>()->objs_colidindo[i] == corpo_obj[corpoA])
			{
				corpo_obj[corpoB]->pegar_componente<box_2D>()->objs_colidindo.erase(corpo_obj[corpoB]->pegar_componente<box_2D>()->objs_colidindo.begin() + i);
				break;
			}
		}
	}
	void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {}
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {}
};

void atualisar_global_box2D()
{
	if (box_2D_iniciado != true)
	{

		mundo.SetContactFilter(new filtro_colisao());
		mundo.SetContactListener(new m_contactlistener());
		passo_tempo = 0;

		box_2D_iniciado = true;
	}

	mundo.SetGravity(converter_b2D(gravidade));

	passo_tempo = (Tempo::tempo - ultimo_tempo) * Tempo::velocidadeTempo;
	// mundo.Step(passo_tempo * Tempo::velocidadeTempo, velocidade_interacoes, iteracao_posicao);
	mundo.Step(passo_tempo, velocidade_interacoes, iteracao_posicao);
	ultimo_tempo = Tempo::tempo;
}

class fisica_char_B2D : public componente
{
public:
	vec2 escala = vec2(0.5, 1);

	bool chao = false, teto = false;
	shared_ptr<objeto_jogo> detec_chao, detec_teto;

	fisica_char_B2D() {}

	void iniciar()
	{

		if (esse_objeto->pegar_componente<box_2D>() != NULL)
		{
			escala = esse_objeto->pegar_componente<box_2D>()->escala;
		}

		detec_chao = novo_objeto_jogo();
		detec_chao->adicionar_componente(transform_(false, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));
		box_2D b2d;
		b2d.gatilho = true;
		b2d.dinamica = dinamico;
		// b2d.escala = vec2(escala.x - 0.1f, 0.1f);
		b2d.escala = vec2(escala.x, 0.1f);
		b2d.forma = caixa;
		detec_chao->adicionar_componente(b2d);
		cena_objetos_selecionados->adicionar_objetos({detec_chao});

		detec_teto = novo_objeto_jogo();
		detec_teto->adicionar_componente(transform_(false, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));
		b2d.gatilho = true;
		b2d.dinamica = dinamico;
		b2d.escala_gravidade = 0;
		// b2d.escala = vec2(escala.x - 0.1f, 0.1f);
		b2d.escala = vec2(escala.x, 0.1f);
		b2d.forma = caixa;
		detec_teto->adicionar_componente(b2d);
		cena_objetos_selecionados->adicionar_objetos({detec_teto});
	}
	void atualisar()
	{
		shared_ptr<box_2D> b2d = esse_objeto->pegar_componente<box_2D>();
		if (b2d != NULL)
		{
			escala = b2d->escala;
		}

		vec3 pos = esse_objeto->pegar_componente<transform_>()->pos;
		

		detec_chao->pegar_componente<box_2D>()->mudar_pos(vec2(pos.x, pos.y - escala.y));
		detec_teto->pegar_componente<box_2D>()->mudar_pos(vec2(pos.x, pos.y + escala.y));

		// chao = (detec_chao->pegar_componente<box_2D>()->objs_colidindo.size() > 1);
		// teto = (detec_teto->pegar_componente<box_2D>()->objs_colidindo.size() > 1);
		int numero_colisoes = 0;
		for (shared_ptr<objeto_jogo> obj : detec_chao->pegar_componente<box_2D>()->objs_colidindo)
		{
			if (obj->pegar_componente<box_2D>() != NULL && obj->pegar_componente<box_2D>()->gatilho == false)
			{
				numero_colisoes++;
			}
		}
		chao = numero_colisoes > 1;

		numero_colisoes = 0;

		for (shared_ptr<objeto_jogo> obj : detec_teto->pegar_componente<box_2D>()->objs_colidindo)
		{
			if (obj->pegar_componente<box_2D>() != NULL && obj->pegar_componente<box_2D>()->gatilho == false)
			{
				numero_colisoes++;
			}
		}
		teto = numero_colisoes > 1;
	}
	void finalisar()
	{
		cena_objetos_selecionados->remover_objeto({detec_chao});
		cena_objetos_selecionados->remover_objeto({detec_teto});
	}
};
