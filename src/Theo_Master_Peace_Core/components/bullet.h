#pragma once
#include <set>
#include "RecursosT.h"
#include "sceane.h"
#include "Tempo.h"
#include "game_object.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

btDiscreteDynamicsWorld *dynamicsWorld = NULL;

map<btCollisionObject *, shared_ptr<objeto_jogo>> collisionObject_obj;

glm::vec3 btToGlm(const btVector3 &v)
{
    return glm::vec3(v.getX(), v.getY(), v.getZ());
}

btVector3 glmToBt(const glm::vec3 &v)
{
    return btVector3(v.x, v.y, v.z);
}

struct physics_3D_collisionInfo
{
    btCollisionObject *objectA;
    btCollisionObject *objectB;
    btVector3 contactPointA;
    btVector3 contactPointB;
    btVector3 normalOnB;
    btScalar appliedImpulse;
};
std::vector<colis_info> physics_3D_collisionInfos;

#include <bullet/btBulletDynamicsCommon.h>

void deleteCollisionObject(btCollisionObject *object)
{
    // Remove the object from the dynamics world
    dynamicsWorld->removeCollisionObject(object);

    // Delete the motion state if the object is a rigid body
    if (btRigidBody::upcast(object))
    {
        btRigidBody *rigidBody = btRigidBody::upcast(object);
        btMotionState *motionState = rigidBody->getMotionState();
        if (motionState)
        {
            delete motionState;
        }
    }

    // Delete the collision shape
    btCollisionShape *shape = object->getCollisionShape();
    if (shape)
    {
        delete shape;
    }

    // Finally, delete the collision object itself
    delete object;
}

void getObjectPositionAndQuaternion(const btCollisionObject *object, glm::vec3 &position, glm::quat &quaternion)
{
    btTransform transform = object->getWorldTransform();
    btVector3 btPosition = transform.getOrigin();
    btQuaternion btQuaternion = transform.getRotation();

    position = glm::vec3(btPosition.getX(), btPosition.getY(), btPosition.getZ());
    quaternion = glm::quat(btQuaternion.getW(), btQuaternion.getX(), btQuaternion.getY(), btQuaternion.getZ());
}

class bullet : public componente
{
public:

    shared_ptr<malha> collision_mesh;
    char forma = caixa;
    float densidade = 1, atrito = 1;
    bool gatilho = false;
    char dinamica = estatico;
    vec3 escala = vec3(1, 1, 1);
    float escala_gravidade = 1;
    bool rotacionarX = true, rotacionarY = true, rotacionarZ = true;
    info_camada layer;

    btCollisionObject *bt_obj = NULL;
    //btTransform transform;

    bullet() {}
    void iniciar() {
        
    }

    void atualisar()
    {
        ///*
        btTransform transform;
        shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
        if (tf != NULL && bt_obj != NULL)
        {
            getObjectPositionAndQuaternion(bt_obj, tf->pos, tf->quater);
        }
        //*/
        
    }

    void colidir(colis_info col) {}

    void finalisar()
    {
        if (bt_obj != NULL)
        {
            deleteCollisionObject(bt_obj);
            collisionObject_obj.erase(bt_obj);
        }
        bt_obj = NULL;
    }

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

class CustomContactResultCallback : public btCollisionWorld::ContactResultCallback
{
public:
    CustomContactResultCallback()
    {
    }

    virtual btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1)
    {
        colis_info collisionInfo;

        collisionInfo.obj = collisionObject_obj[const_cast<btCollisionObject *>(colObj0Wrap->getCollisionObject())].get();
        collisionInfo.cos_obj = collisionObject_obj[const_cast<btCollisionObject *>(colObj1Wrap->getCollisionObject())].get();
        collisionInfo.pos = btToGlm(cp.getPositionWorldOnA());
        collisionInfo.nor = btToGlm(cp.m_normalWorldOnB);
        collisionInfo.velocidade = cp.m_appliedImpulse;
        physics_3D_collisionInfos.push_back(collisionInfo);

        collisionInfo.cos_obj = collisionObject_obj[const_cast<btCollisionObject *>(colObj0Wrap->getCollisionObject())].get();
        collisionInfo.obj = collisionObject_obj[const_cast<btCollisionObject *>(colObj1Wrap->getCollisionObject())].get();
        physics_3D_collisionInfos.push_back(collisionInfo);

        return 0; // return 0 to process all collisions
    }
};

class CustomOverlapFilterCallback : public btOverlapFilterCallback
{
public:
    virtual bool needBroadphaseCollision(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) const
    {
        btCollisionObject *obj0 = static_cast<btCollisionObject *>(proxy0->m_clientObject);
        btCollisionObject *obj1 = static_cast<btCollisionObject *>(proxy1->m_clientObject);

        shared_ptr<bullet> bullet1 = collisionObject_obj[obj0]->pegar_componente<bullet>();
        shared_ptr<bullet> bullet2 = collisionObject_obj[obj1]->pegar_componente<bullet>();
        for (int i : bullet1->layer.camada_colide)
        {
            if (i == bullet2->layer.camada)
            {
                return true;
            }
        }

        return false;
    }
};

bool raycast_bullet_3D(vec3 rayFrom, vec3 rayTo, colis_info &result)
{

    btCollisionWorld::ClosestRayResultCallback rayCallback(glmToBt(rayFrom), glmToBt(rayTo));
    dynamicsWorld->rayTest(glmToBt(rayFrom), glmToBt(rayTo), rayCallback);

    if (rayCallback.hasHit())
    {
        result.pos = btToGlm(rayCallback.m_hitPointWorld);
        result.nor = btToGlm(rayCallback.m_hitNormalWorld);
        result.cos_obj = collisionObject_obj[const_cast<btCollisionObject *>(rayCallback.m_collisionObject)].get();
        if (rayCallback.hasHit())
        {
            return true;
        }
    }
    return false;
}

class bullet_charter : public componente
{
public:
    bullet_charter() {}
    void iniciar() {}
    void atualisar() {}
    void colidir(colis_info col) {}
    void finalisar() {}
};

void iniciar_global_bullet()
{
    btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btDbvtBroadphase *broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    CustomOverlapFilterCallback filterCallback;
    dynamicsWorld->getPairCache()->setOverlapFilterCallback(&filterCallback);
}

void get_3D_collisions()
{
    CustomContactResultCallback callback;

    for (int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++)
    {
        btCollisionObject *objA = dynamicsWorld->getCollisionObjectArray()[i];
        for (int j = i + 1; j < dynamicsWorld->getNumCollisionObjects(); j++)
        {
            btCollisionObject *objB = dynamicsWorld->getCollisionObjectArray()[j];
            if (dynamicsWorld->getDispatcher()->needsCollision(objA, objB))
            {
                dynamicsWorld->contactPairTest(objA, objB, callback);
            }
        }
    }
}

void applay_3D_collisions()
{
    for (colis_info ci : physics_3D_collisionInfos)
    {
        objeto_jogo *obj = (objeto_jogo *)ci.obj;
        obj->colidir(ci);
    }
}

float bullet_passo_tempo;
float bullet_ultimo_tempo;
bool global_bullet_iniciado = false;

void atualisar_global_bullet()
{
    if (!global_bullet_iniciado)
    {
        iniciar_global_bullet();
        global_bullet_iniciado = true;
    }

    // colisions
    get_3D_collisions();
    applay_3D_collisions();
    std::vector<colis_info> vazio = {};
    physics_3D_collisionInfos.swap(vazio);

    bullet_passo_tempo = 0;

    dynamicsWorld->setGravity(glmToBt(gravidade));

    bullet_passo_tempo = (Tempo::tempo - bullet_ultimo_tempo) * Tempo::velocidadeTempo;
    dynamicsWorld->stepSimulation(bullet_passo_tempo * Tempo::velocidadeTempo, 10);
    bullet_ultimo_tempo = Tempo::tempo;
}
