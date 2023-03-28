#pragma once
#include <set>
#include "RecursosT.h"
#include "sceane.h"
#include "Tempo.h"
#include "game_object.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace physics_3D_test_area
{

    struct RaycastResult
    {
        bool hasHit;
        btVector3 hitPoint;
        btVector3 hitNormal;
        btScalar hitFraction;
        btCollisionObject *hitObject;
        int partId;
        int triangleIndex;
    };

    RaycastResult raycast_bullet_3D(btDiscreteDynamicsWorld *dynamicsWorld, const btVector3 &rayFrom, const btVector3 &rayTo)
    {
        RaycastResult result;
        result.hasHit = false;

        btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
        dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);

        if (rayCallback.hasHit())
        {
            result.hasHit = true;
            result.hitPoint = rayCallback.m_hitPointWorld;
            result.hitNormal = rayCallback.m_hitNormalWorld;
            result.hitFraction = rayCallback.m_closestHitFraction;
            result.hitObject = const_cast<btCollisionObject *>(rayCallback.m_collisionObject);
            result.partId = rayCallback.m_collisionFilterGroup;
            result.triangleIndex = rayCallback.m_collisionFilterMask;
        }

        return result;
    }

    btDiscreteDynamicsWorld *dynamicsWorld;

    struct bullet_mesh
    {
        vector<vec3> vertices;
        vector<unsigned int> indices;
    };

    struct physics_3D_collisionInfo
    {
        btCollisionObject *objectA;
        btCollisionObject *objectB;
        btVector3 contactPointA;
        btVector3 contactPointB;
        btVector3 normalOnB;
        btScalar appliedImpulse;
    };
    std::vector<physics_3D_collisionInfo> collisionInfos;

    class CustomCollisionCallback : public btCollisionWorld::ContactResultCallback
    {
    public:
        CustomCollisionCallback()
        {
        }

        virtual btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1)
        {
            physics_3D_collisionInfo collisionInfo;

            collisionInfo.objectA = const_cast<btCollisionObject *>(colObj0Wrap->getCollisionObject());
            collisionInfo.objectB = const_cast<btCollisionObject *>(colObj1Wrap->getCollisionObject());
            collisionInfo.contactPointA = cp.getPositionWorldOnA();
            collisionInfo.contactPointB = cp.getPositionWorldOnB();
            collisionInfo.normalOnB = cp.m_normalWorldOnB;
            collisionInfo.appliedImpulse = cp.m_appliedImpulse;

            collisionInfos.push_back(collisionInfo);

            return 0; // return 0 to process all collisions
        }
    };

#include <bullet/btBulletDynamicsCommon.h>

    class CustomOverlapFilterCallback : public btOverlapFilterCallback
    {
    public:
        virtual bool needBroadphaseCollision(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) const
        {
            // Retrieve the btCollisionObject pointers from the user pointers
            btCollisionObject *obj0 = static_cast<btCollisionObject *>(proxy0->m_clientObject);
            btCollisionObject *obj1 = static_cast<btCollisionObject *>(proxy1->m_clientObject);

            // Implement your custom logic to decide whether obj0 and obj1 should be allowed to collide
            // Return true to allow collision, return false to disallow collision

            return true; // By default, allow all collisions
        }
    };

    void set_custom_collision_test(btCollisionObject *A, btCollisionObject *B)
    {
        CustomCollisionCallback callback;
        dynamicsWorld->contactPairTest(A, B, callback);

        CustomOverlapFilterCallback filterCallback;
        dynamicsWorld->getPairCache()->setOverlapFilterCallback(&filterCallback);
    }

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

    btRigidBody *addMeshToDynamicsWorld(btDiscreteDynamicsWorld *dynamicsWorld, const bullet_mesh &mesh, float mass, const btVector3 &position)
    {
        btTriangleMesh *triangleMesh = new btTriangleMesh();

        for (size_t i = 0; i < mesh.indices.size(); i += 3)
        {
            btVector3 vertex0(mesh.vertices[mesh.indices[i]].x, mesh.vertices[mesh.indices[i]].y, mesh.vertices[mesh.indices[i]].z);
            btVector3 vertex1(mesh.vertices[mesh.indices[i + 1]].x, mesh.vertices[mesh.indices[i + 1]].y, mesh.vertices[mesh.indices[i + 1]].z);
            btVector3 vertex2(mesh.vertices[mesh.indices[i + 2]].x, mesh.vertices[mesh.indices[i + 2]].y, mesh.vertices[mesh.indices[i + 2]].z);

            triangleMesh->addTriangle(vertex0, vertex1, vertex2);
        }

        btCollisionShape *meshShape = new btBvhTriangleMeshShape(triangleMesh, true);
        btDefaultMotionState *meshMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), position));
        btVector3 meshInertia(0, 0, 0);
        meshShape->calculateLocalInertia(mass, meshInertia);
        btRigidBody::btRigidBodyConstructionInfo meshRigidBodyCI(mass, meshMotionState, meshShape, meshInertia);
        btRigidBody *meshRigidBody = new btRigidBody(meshRigidBodyCI);

        dynamicsWorld->addRigidBody(meshRigidBody);
        return meshRigidBody;
    }

    btPairCachingGhostObject *createBoxTrigger(btDiscreteDynamicsWorld *dynamicsWorld, const btVector3 &halfExtents, const btVector3 &position, const btQuaternion &orientation)
    {
        btCollisionShape *boxShape = new btBoxShape(halfExtents);

        btTransform triggerTransform;
        triggerTransform.setIdentity();
        triggerTransform.setOrigin(position);
        triggerTransform.setRotation(orientation);

        btPairCachingGhostObject *ghostObject = new btPairCachingGhostObject();
        ghostObject->setCollisionShape(boxShape);
        ghostObject->setWorldTransform(triggerTransform);
        ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

        dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);

        return ghostObject;
    }

    void delete_bullet_obiject(btRigidBody *bullet_obiject)
    {
        dynamicsWorld->removeRigidBody(bullet_obiject);
        delete bullet_obiject->getMotionState();
        delete bullet_obiject->getCollisionShape();
        delete bullet_obiject;
    }

    void delete_world(btRigidBody *sphereRigidBody, btRigidBody *groundRigidBody, btDiscreteDynamicsWorld *dynamicsWorld)
    {
        // delete_bullet_obiject(sphereRigidBody);
        // delete_bullet_obiject(groundRigidBody);

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
        dynamicsWorld = initPhysicsWorld();

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

        delete_world(sphereRigidBody, groundRigidBody, dynamicsWorld);
    }
};

btDiscreteDynamicsWorld *dynamicsWorld;

void delete_bullet_obiject(btRigidBody *bullet_obiject)
{
    dynamicsWorld->removeRigidBody(bullet_obiject);
    delete bullet_obiject->getMotionState();
    delete bullet_obiject->getCollisionShape();
    delete bullet_obiject;
}

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

    void atualisar()
    {
        for (colis_info c : colis_infos)
        {
            esse_objeto->colidir(c);
        }
        vector<colis_info> vazio;
        colis_infos.swap(vazio);
    }

    void colidir(colis_info col) {}

    void finalisar() {}

    void aplay()
    {
        finalisar();
        iniciar();
    }

    ~bullet()
    {
        finalisar();
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

float bullet_passo_tempo;
float bullet_ultimo_tempo;
void atualisar_global_bullet()
{
    if (box_2D_iniciado != true)
    {

        // mundo.SetContactFilter(new filtro_colisao());
        // mundo.SetContactListener(new m_contactlistener());
        bullet_passo_tempo = 0;

        box_2D_iniciado = true;
    }

    mundo.SetGravity(converter(gravidade));

    bullet_passo_tempo = (Tempo::tempo - bullet_ultimo_tempo) * Tempo::velocidadeTempo;
    // mundo.Step(bullet_passo_tempo * Tempo::velocidadeTempo, velocidade_interacoes, iteracao_posicao);
    // mundo.Step(bullet_passo_tempo, velocidade_interacoes, iteracao_posicao);
    bullet_ultimo_tempo = Tempo::tempo;
}
