#pragma once
#include <set>
#include "RecursosT.h"
#include "sceane.h"
#include "Tempo.h"
#include "game_object.h"

#include <btBulletDynamicsCommon.h>

/*
struct mesh {
    vector<vec3> vertices;
    vector<unsigned int> index
};



*/
namespace physics_3D_test_area
{

    btDiscreteDynamicsWorld *initPhysicsWorld()
    {
        btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btDbvtBroadphase *broadphase = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
        btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
        return dynamicsWorld;
    }

    // Create a ground rigid body
    btRigidBody *createGround(btDiscreteDynamicsWorld *dynamicsWorld)
    {
        btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
        btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);
        dynamicsWorld->addRigidBody(groundRigidBody);
        return groundRigidBody;
    }

    // Create a sphere rigid body
    btRigidBody *createSphere(btDiscreteDynamicsWorld *dynamicsWorld, btScalar mass, btScalar radius, const btVector3 &position)
    {
        btCollisionShape *sphereShape = new btSphereShape(radius);
        btDefaultMotionState *sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), position));
        btVector3 sphereInertia(0, 0, 0);
        sphereShape->calculateLocalInertia(mass, sphereInertia);
        btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, sphereInertia);
        btRigidBody *sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
        dynamicsWorld->addRigidBody(sphereRigidBody);
        return sphereRigidBody;
    }

    void delete_world(btRigidBody *sphereRigidBody,btRigidBody *groundRigidBody,btDiscreteDynamicsWorld *dynamicsWorld){
        delete sphereRigidBody->getMotionState();
        delete sphereRigidBody->getCollisionShape();
        delete sphereRigidBody;
        delete groundRigidBody->getMotionState();
        delete groundRigidBody->getCollisionShape();
        delete groundRigidBody;

        btCollisionDispatcher *dispatcher = static_cast<btCollisionDispatcher *>(dynamicsWorld->getDispatcher());
        btDefaultCollisionConfiguration *collisionConfiguration = static_cast<btDefaultCollisionConfiguration *>(dispatcher->getCollisionConfiguration());
        delete dynamicsWorld->getBroadphase();
        delete dynamicsWorld->getConstraintSolver();
        delete dispatcher;
        delete collisionConfiguration;
        delete dynamicsWorld;
    }

    void physics_3D_test()
    {
        btDiscreteDynamicsWorld *dynamicsWorld = initPhysicsWorld();

        btRigidBody *groundRigidBody = createGround(dynamicsWorld);

        btScalar sphereMass = 1.0;
        btScalar sphereRadius = 1.0;
        btVector3 spherePosition(0, 10, 0);
        btRigidBody *sphereRigidBody = createSphere(dynamicsWorld, sphereMass, sphereRadius, spherePosition);

        for (int i = 0; i < 300; ++i)
        {
            dynamicsWorld->stepSimulation(1.0 / 60.0, 10);

            btTransform sphereTransform;
            sphereRigidBody->getMotionState()->getWorldTransform(sphereTransform);
            btVector3 spherePosition = sphereTransform.getOrigin();
            std::cout << "Sphere position: " << spherePosition.getX() << ", " << spherePosition.getY() << ", " << spherePosition.getZ() << std::endl;
        }

        dynamicsWorld->removeRigidBody(sphereRigidBody);
        dynamicsWorld->removeRigidBody(groundRigidBody);

        delete_world(sphereRigidBody,groundRigidBody,dynamicsWorld);
    }
};

class bullet : public componente
{
public:
    shared_ptr<malha> collision_mesh;
    char forma = caixa;
    float densidade = 1, atrito = 0;
    bool gatilho = false;
    char dinamica = estatico;
    vec3 escala = vec3(1, 1, 1);
    float escala_gravidade = 1;
    bool rotacionar = true;
    info_camada layer;
    vector<colis_info> colis_infos;
    vector<shared_ptr<objeto_jogo>> objects_colidinf;

    bullet() {}
    void iniciar() {}

    void atualisar() {
        for (colis_info c : colis_infos) {
			esse_objeto->colidir(c);
		}
		vector<colis_info> vazio;
		colis_infos.swap(vazio);
    }

    void colidir(colis_info col) {}
    
    void finalisar() {}
    
    void aplay(){
        finalisar();
        iniciar();
    }
};

class bullet_charter : public componente
{
public:
    bullet_charter() {}
    void iniciar() {}
    void atualisar() {}
    void colidir(colis_info col) {}
    void finalisar() {}
};

float passo_tempo;
float ultimo_tempo;
void atualisar_global_bullet()
{
    if (box_2D_iniciado != true)
    {

        // mundo.SetContactFilter(new filtro_colisao());
        // mundo.SetContactListener(new m_contactlistener());
        passo_tempo = 0;

        box_2D_iniciado = true;
    }

    mundo.SetGravity(converter(gravidade));

    passo_tempo = (Tempo::tempo - ultimo_tempo) * Tempo::velocidadeTempo;
    // mundo.Step(passo_tempo * Tempo::velocidadeTempo, velocidade_interacoes, iteracao_posicao);
    // mundo.Step(passo_tempo, velocidade_interacoes, iteracao_posicao);
    ultimo_tempo = Tempo::tempo;
}
