#pragma once
#include <set>
#include "RecursosT.h"
#include "scene.h"
#include "Tempo.h"
#include "game_object.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

btDiscreteDynamicsWorld *dynamicsWorld;

int global_bullet_iniciado = 0;

/*
struct Bullet_Mesh
{
    std::vector<btVector3> vertices = {};
    std::vector<int> indices = {};
};
map<shared_ptr<objeto_jogo>, Bullet_Mesh> Bullet_Meshes;
*/
map<shared_ptr<objeto_jogo>, btTriangleMesh *> triangleMeshs;

map<btCollisionObject *, shared_ptr<objeto_jogo>> collisionObject_obj;

map<objeto_jogo *, vector<objeto_jogo *>> bu_collisions_no_per_object;

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

        if (collisionObject_obj.find(object) != collisionObject_obj.end())
        {
            delete triangleMeshs[collisionObject_obj[object]];
            triangleMeshs.erase(collisionObject_obj[object]);
        }
    }

    // Finally, delete the collision object itself
    delete object;
}

void getObjectPositionAndQuaternion(const btCollisionObject *object, glm::vec3 &position, glm::quat &quaternion)
{
    btTransform transform;
    if (object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
    {
        ((btRigidBody *)(object))->getMotionState()->getWorldTransform(transform);
    }
    else
    {
        transform = object->getWorldTransform();
    }
    btVector3 btPosition = transform.getOrigin();
    btQuaternion btQuaternion = transform.getRotation();
    position = glm::vec3(btPosition.getX(), btPosition.getY(), btPosition.getZ());

    quaternion = glm::quat(btQuaternion.getW(), btQuaternion.getX(), btQuaternion.getY(), btQuaternion.getZ());
}

btTransform getObjectTransform(const btCollisionObject *object)
{
    btTransform transform;
    if (object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
    {
        ((btRigidBody *)(object))->getMotionState()->getWorldTransform(transform);
    }
    else
    {
        transform = object->getWorldTransform();
    }
    return transform;
}
void iniciar_global_bullet();

class bullet : public componente
{
public:
    shared_ptr<malha> collision_mesh;
    char forma = 0;
    float densidade = 1, atrito = 1;
    bool gatilho = false;
    char dinamica = estatico;
    vec3 escala = vec3(1, 1, 1);
    float escala_gravidade = 1;
    bool rotacionarX = true, rotacionarY = true, rotacionarZ = true;
    info_camada layer;

    btCollisionObject *bt_obj = NULL;

    void iniciar()
    {
        bu_collisions_no_per_object[esse_objeto.get()] = {};
        iniciar_global_bullet();
        btCollisionShape *Shape;
        if (forma == caixa)
        {
            Shape = new btBoxShape(glmToBt(escala));
        }
        else if (forma == esfera)
        {
            Shape = new btSphereShape(escala.x);
        }
        else if (forma == convexo)
        {

            if (collision_mesh != NULL)
            {

                if (dinamica == estatico)
                {
                    btTriangleMesh *tm = new btTriangleMesh();
                    for (int i = 0; i < collision_mesh->indice.size(); i += 3)
                    {

                        int index = collision_mesh->indice[i];
                        vertice v = collision_mesh->vertices[index];
                        btVector3 vertex1 = btVector3(v.posicao[0], v.posicao[1], v.posicao[2]);

                        index = collision_mesh->indice[i + 1];
                        v = collision_mesh->vertices[index];
                        btVector3 vertex2 = btVector3(v.posicao[0], v.posicao[1], v.posicao[2]);

                        index = collision_mesh->indice[i + 2];
                        v = collision_mesh->vertices[index];
                        btVector3 vertex3 = btVector3(v.posicao[0], v.posicao[1], v.posicao[2]);

                        tm->addTriangle(vertex1, vertex2, vertex3);
                    }

                    Shape = new btBvhTriangleMeshShape(tm, true);

                    triangleMeshs[esse_objeto] = tm;
                }
                else
                {
                    btConvexHullShape *convexHullShape = new btConvexHullShape();

                    for (int i = 0; i < collision_mesh->vertices.size(); i++)
                    {
                        convexHullShape->addPoint(btVector3(collision_mesh->vertices[i].posicao[0], collision_mesh->vertices[i].posicao[1], collision_mesh->vertices[i].posicao[2]));
                    }

                    Shape = convexHullShape;
                }
            }
            else
            {
                cout << "fail load collision mesh\n";
                if (triangleMeshs.find(esse_objeto) != triangleMeshs.end())
                {
                    delete triangleMeshs[esse_objeto];
                    triangleMeshs.erase(esse_objeto);
                }

                Shape = new btBoxShape(glmToBt(escala));
            }
        }

        btTransform transform;
        transform.setIdentity();
        vec3 position = vec3(0, 0, 0);
        transform.setOrigin(glmToBt(position));
        quat quaternion;
        shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
        if (tf != NULL)
        {
            transform.setOrigin(glmToBt(tf->pos));
            transform.setRotation(btQuaternion(tf->quater.x, tf->quater.y, tf->quater.z, tf->quater.w));
            position = tf->pos;
            quaternion = tf->quater;
        }

        if (gatilho)
        {
            bt_obj = new btGhostObject();
            bt_obj->setCollisionShape(Shape);
            bt_obj->setCollisionFlags(bt_obj->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
            bt_obj->setWorldTransform(transform);
            dynamicsWorld->addCollisionObject(bt_obj, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter);
        }
        else
        {
            btDefaultMotionState *MotionState = new btDefaultMotionState(transform);
            if (dinamica == dinamico)
            {
                btVector3 Inertia = btVector3(0, 0, 0);
                Shape->calculateLocalInertia(densidade, Inertia);
                btRigidBody::btRigidBodyConstructionInfo CI(densidade, MotionState, Shape, Inertia);
                btRigidBody *rb = new btRigidBody(CI);
                rb->setAngularFactor(btVector3(rotacionarX, rotacionarY, rotacionarZ));
                dynamicsWorld->addRigidBody(rb);
                bt_obj = rb;
            }
            else if (dinamica == estatico)
            {
                btRigidBody::btRigidBodyConstructionInfo CI(0, MotionState, Shape, btVector3(0, 0, 0));
                btRigidBody *rb = new btRigidBody(CI);
                dynamicsWorld->addRigidBody(rb);
                bt_obj = rb;
            }
        }

        collisionObject_obj.insert(pair<btCollisionObject *, shared_ptr<objeto_jogo>>(bt_obj, esse_objeto));
    }

    void atualisar()
    {
        shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
        if (tf != NULL && bt_obj != NULL)
        {
            getObjectPositionAndQuaternion(bt_obj, tf->pos, tf->quater);
        }
    }

    void colidir(colis_info col) {}

    void finalisar()
    {
        if (bu_collisions_no_per_object.find(esse_objeto.get()) != bu_collisions_no_per_object.end())
        {
            for (objeto_jogo *obj : bu_collisions_no_per_object[esse_objeto.get()])
            {
                vector<objeto_jogo *> vazio = {};
                bu_collisions_no_per_object[esse_objeto.get()].swap(vazio);
            }
            bu_collisions_no_per_object.erase(esse_objeto.get());
        }

        if (bt_obj != NULL)
        {
            ///*
            if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
            {
                dynamicsWorld->removeRigidBody((btRigidBody *)bt_obj);
            }
            else
            {
                dynamicsWorld->removeCollisionObject(bt_obj);
            }
            //*/
            deleteCollisionObject(bt_obj);
            if (collisionObject_obj.find(bt_obj) != collisionObject_obj.end())
            {
                collisionObject_obj.erase(bt_obj);
            }
            bt_obj = NULL;
        }
    }

    void aplay()
    {
        finalisar();
        iniciar();
    }

    void mudar_pos(vec3 pos)
    {
        btTransform tf = getObjectTransform(bt_obj);
        tf.setOrigin(btVector3(pos.x, pos.y, pos.z));
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            ((btRigidBody *)(bt_obj))->getMotionState()->setWorldTransform(tf);
        }
        else
        {
            bt_obj->setWorldTransform(tf);
        }
    }
    void mudar_rot(quat rot)
    {
        btTransform tf = getObjectTransform(bt_obj);
        tf.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            ((btRigidBody *)(bt_obj))->getMotionState()->setWorldTransform(tf);
        }
        else
        {
            bt_obj->setWorldTransform(tf);
        }
    }
    void mudar_rot(vec3 rot)
    {
        mudar_rot(graus_quat(rot));
    }
    void adicionar_forca(vec3 forca)
    {
        ((btRigidBody *)(bt_obj))->applyCentralForce(btVector3(forca.x, forca.y, forca.z));
    }
    void adicionar_impulso(vec3 forca)
    {
        ((btRigidBody *)(bt_obj))->applyCentralImpulse(btVector3(forca.x, forca.y, forca.z));
    }

    void adicionar_forca_rotativo(vec3 forca)
    {
        ((btRigidBody *)(bt_obj))->applyTorque(btVector3(forca.x, forca.y, forca.z));
    }
    void adicionar_impulso_rotativo(vec3 forca)
    {
        ((btRigidBody *)(bt_obj))->applyTorqueImpulse(btVector3(forca.x, forca.y, forca.z));
    }
    ~bullet()
    {
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

        if (collisionObject_obj.find(obj0) != collisionObject_obj.end() && collisionObject_obj.find(obj1) != collisionObject_obj.end())
        {
            shared_ptr<bullet> bullet1 = collisionObject_obj[obj0]->pegar_componente<bullet>();
            shared_ptr<bullet> bullet2 = collisionObject_obj[obj1]->pegar_componente<bullet>();

            if (bullet1 != NULL && bullet2 != NULL)
            {

                for (int i : bullet1->layer.camada_colide)
                {
                    if (i == bullet2->layer.camada)
                    {
                        return true;
                    }
                }
            }
        }
        else
        {
            return true;
        }
        return false;
    }
};

bool raycast_dir_bullet_3D(vec3 rayFrom, vec3 rayTo, colis_info &result)
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
    shared_ptr<objeto_jogo> roof_cheker, floor_cheker;
    bool in_roof, in_floor;

    bullet_charter() {}
    void iniciar() {}
    void atualisar() {}
    void colidir(colis_info col) {}
    void finalisar() {}
};

void iniciar_global_bullet()
{
    if (global_bullet_iniciado == 0)
    {
        cout << "iniciar global bullet\n";

        btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btDbvtBroadphase *broadphase = new btDbvtBroadphase();
        CustomOverlapFilterCallback *customFilterCallback = new CustomOverlapFilterCallback();
        broadphase->getOverlappingPairCache()->setOverlapFilterCallback(customFilterCallback);
        btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

        global_bullet_iniciado++;
    }
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

        colis_info ci2 = ci;
        ci2.obj = ci.cos_obj;
        ci2.cos_obj = ci.obj;
        obj = (objeto_jogo *)ci2.obj;
        obj->colidir(ci);
    }
}
void clean_collisions()
{
    std::vector<colis_info> vazio = {};
    physics_3D_collisionInfos.swap(vazio);
}

void clean_bu_collisions_no_per_object()
{
    for (pair<objeto_jogo *, std::vector<objeto_jogo *>> p : bu_collisions_no_per_object)
    {
        vector<objeto_jogo *> empt;
        // print({"bu_collisions_no_per_object[p.first].size()",bu_collisions_no_per_object[p.first].size()});
        bu_collisions_no_per_object[p.first].swap(empt);
    }
}

void get_bu_collisions_no_per_object()
{
    for (colis_info ci : physics_3D_collisionInfos)
    {
        bu_collisions_no_per_object[(objeto_jogo *)ci.obj].push_back((objeto_jogo *)ci.cos_obj);
    }
}

float bullet_passo_tempo;
float bullet_ultimo_tempo;
int maxSubSteps = 10;

void atualisar_global_bullet()
{
    iniciar_global_bullet();

    // collisions
    get_3D_collisions();
    applay_3D_collisions();
    clean_bu_collisions_no_per_object();
    get_bu_collisions_no_per_object();
    clean_collisions();

    bullet_passo_tempo = 0;

    dynamicsWorld->setGravity(glmToBt(gravidade));

    bullet_passo_tempo = (Tempo::tempo - bullet_ultimo_tempo) * Tempo::velocidadeTempo;
    // dynamicsWorld->stepSimulation((bullet_passo_tempo * Tempo::velocidadeTempo), maxSubSteps, bullet_passo_tempo * Tempo::velocidadeTempo);
    // dynamicsWorld->stepSimulation(1 / 60.f, 1);
    dynamicsWorld->stepSimulation((bullet_passo_tempo * Tempo::velocidadeTempo), maxSubSteps);
    bullet_ultimo_tempo = Tempo::tempo;
}
