#pragma once
#include <set>
#include "RecursosT.h"
#include "scene.h"
#include "Tempo.h"
#include "game_object.h"
#include <thread>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <cmath>

#include "render_mesh.h"
#include "ManusearArquivos.h"

#include <btBulletDynamicsCommon.h>
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.cpp"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>



btDiscreteDynamicsWorld *dynamicsWorld;

int global_bullet_iniciado = 0;

vector<thread> bullet_threads;

int btMeshes_shapes_count = 0;
std::map<shared_ptr<std::string>, btCollisionShape *> btMeshes_shapes;
std::map<string, shared_ptr<std::string>> btMeshes_shapes_addresses;
void btClean_Meshes_shapes()
{

    std::map<shared_ptr<std::string>, btCollisionShape *> mapa2;
    std::map<string, shared_ptr<std::string>> mapa3;

    for (pair<shared_ptr<std::string>, btCollisionShape *> p : btMeshes_shapes)
    {

        if (p.first.use_count() > 3)
        {
            mapa2.insert(p);
            mapa3.insert(pair<string, shared_ptr<std::string>>(*p.first.get(), p.first));
        }
        else
        {
            delete p.second;
            btMeshes_shapes_count -= 1;
        }
    }
    btMeshes_shapes.clear();
    btMeshes_shapes_addresses.clear();
}

shared_ptr<std::string> get_mesh_shape_address(std::string addres)
{
    if (btMeshes_shapes_addresses.find(addres) != btMeshes_shapes_addresses.end())
    {
        return btMeshes_shapes_addresses[addres];
    }
    else
    {
        shared_ptr<std::string> ret = make_shared<std::string>(addres);
        btMeshes_shapes_addresses.insert(pair<string, shared_ptr<std::string>>(addres, ret));
        return ret;
    }
}

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

    btClean_Meshes_shapes();

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
private:
public:
    shared_ptr<malha> collision_mesh;
    char forma = 0;
    float densidade = 1, atrito = 1;
    bool gatilho = false;
    char dinamica = estatico;
    vec3 escala = vec3(1, 1, 1);
    float escala_gravidade = 1;
    bool rotacionarX = true, rotacionarY = true, rotacionarZ = true;
    float elasticidade = 0;
    float gravity_force = 1;
    info_camada layer;
    btCollisionObject *bt_obj = NULL;
    btRigidBody *bt_obj_rb = NULL;
    btCompoundShape *compound = NULL;
    shared_ptr<std::string> mesh_shape_address = NULL;
    vector<objeto_jogo *> objs_touching;
    vector<colis_info> colis_infos;
    bool get_collision_info = false;
    btTransform transform;
    btCollisionShape *Shape;

    void iniciar()
    {
        bu_collisions_no_per_object[esse_objeto.get()] = {};

        transform.setIdentity();

        if (forma == formato_colisao::caixa)
        {
            mesh_shape_address = get_mesh_shape_address("box:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btBoxShape(glmToBt(escala));
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::esfera)
        {
            mesh_shape_address = get_mesh_shape_address("sphere:" + std::to_string(escala.x));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btSphereShape(escala.x);
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::cilindro)
        {
            mesh_shape_address = get_mesh_shape_address("cylinder:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btCylinderShape(glmToBt(escala));
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::capsula)
        {
            mesh_shape_address = get_mesh_shape_address("capsule:" + std::to_string(escala.x) + ":" + std::to_string(escala.y));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btCapsuleShape(escala.x, escala.y);
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::convexo)
        {

            if (collision_mesh != NULL)
            {

                if (dinamica == estatico)
                {
                    mesh_shape_address = get_mesh_shape_address(collision_mesh->arquivo_origem + ":" + collision_mesh->nome + ":static:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));

                    if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
                    {
                        Shape = btMeshes_shapes[mesh_shape_address];
                    }
                    else
                    {
                        btTriangleMesh *tm = new btTriangleMesh();
                        for (int i = 0; i < collision_mesh->indice.size(); i += 3)
                        {

                            int index = collision_mesh->indice[i];
                            vertice v = collision_mesh->vertices[index];
                            btVector3 vertex1 = btVector3(v.posicao[0] * escala.x, v.posicao[1] * escala.y, v.posicao[2] * escala.z);

                            index = collision_mesh->indice[i + 1];
                            v = collision_mesh->vertices[index];
                            btVector3 vertex2 = btVector3(v.posicao[0] * escala.x, v.posicao[1] * escala.y, v.posicao[2] * escala.z);

                            index = collision_mesh->indice[i + 2];
                            v = collision_mesh->vertices[index];
                            btVector3 vertex3 = btVector3(v.posicao[0] * escala.x, v.posicao[1] * escala.y, v.posicao[2] * escala.z);

                            tm->addTriangle(vertex1, vertex2, vertex3);
                        }

                        Shape = new btBvhTriangleMeshShape(tm, true);
                        btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                        btMeshes_shapes_count++;
                    }
                }
                else
                {
                    mesh_shape_address = get_mesh_shape_address(std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z) + ":" + collision_mesh->arquivo_origem + ":" + collision_mesh->nome + ":dynamic");

                    if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
                    {
                        Shape = btMeshes_shapes[mesh_shape_address];
                    }
                    else
                    {
                        btConvexHullShape *convexHullShape = new btConvexHullShape();

                        for (int i = 0; i < collision_mesh->vertices.size(); i++)
                        {
                            convexHullShape->addPoint(btVector3(collision_mesh->vertices[i].posicao[0] * escala.x, collision_mesh->vertices[i].posicao[1] * escala.y, collision_mesh->vertices[i].posicao[2] * escala.z));
                        }

                        Shape = convexHullShape;
                        btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                        btMeshes_shapes_count++;
                    }
                }
            }
            else
            {
                print("fail load collision mesh");

                mesh_shape_address = get_mesh_shape_address("box:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));
                if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
                {
                    Shape = btMeshes_shapes[mesh_shape_address];
                }
                else
                {
                    Shape = new btBoxShape(glmToBt(escala));
                    btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                    btMeshes_shapes_count++;
                }
            }
        }

        vec3 position = vec3(0, 0, 0);
        transform.setOrigin(glmToBt(position));
        quat quaternion;
        compound = new btCompoundShape();

        shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
        if (tf != NULL)
        {

            quaternion = tf->quater;

            transform.setOrigin(glmToBt(tf->pos));
            transform.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
        }

        if (gatilho)
        {

            bt_obj = new btPairCachingGhostObject();
            bt_obj->setCollisionShape(Shape);
            bt_obj->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
            bt_obj->setWorldTransform(transform);
            // dynamicsWorld->addCollisionObject(bt_obj, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter);
            dynamicsWorld->addCollisionObject(bt_obj, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::KinematicFilter);
        }
        else
        {
            btDefaultMotionState *MotionState = new btDefaultMotionState(transform);
            if (dinamica == dinamico || dinamica == cinematico)
            {
                btVector3 Inertia = btVector3(0, 0, 0);
                Shape->calculateLocalInertia(densidade, Inertia);
                btRigidBody::btRigidBodyConstructionInfo CI(densidade, MotionState, Shape, Inertia);
                bt_obj_rb = new btRigidBody(CI);
                bt_obj_rb->setAngularFactor(btVector3(rotacionarX, rotacionarY, rotacionarZ));
                bt_obj_rb->setRestitution(elasticidade);
                bt_obj_rb->setGravity(btVector3(0, 0, 0));
                bt_obj_rb->setFriction(atrito);
                bt_obj_rb->setActivationState(DISABLE_DEACTIVATION);
                dynamicsWorld->addRigidBody(bt_obj_rb);
                bt_obj = bt_obj_rb;
                bt_obj_rb->setGravity(btVector3(gravidade.x * gravity_force, gravidade.y * gravity_force, gravidade.z * gravity_force));
            }
            else if (dinamica == estatico)
            {

                shared_ptr<bullet> pai_bu;
                shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();

                shared_ptr<transform_> pai_tf = esse_objeto->pai->pegar_componente<transform_>();

                if (tf != NULL && esse_objeto->pegar_componente<transform_>() != NULL && esse_objeto->pai->pegar_componente<bullet>() != NULL)
                {

                    pai_bu = esse_objeto->pai->pegar_componente<bullet>();
                }

                btRigidBody::btRigidBodyConstructionInfo CI(0, MotionState, Shape);
                bt_obj_rb = new btRigidBody(CI);

                dynamicsWorld->addRigidBody(bt_obj_rb);
                bt_obj = bt_obj_rb;

                //
            }
            else if (dinamica == cinematico)
            {

                btVector3 Inertia = btVector3(0, 0, 0);
                Shape->calculateLocalInertia(densidade, Inertia);
                btRigidBody::btRigidBodyConstructionInfo CI(0, MotionState, Shape, Inertia);
                bt_obj_rb = new btRigidBody(CI);
                bt_obj_rb->setAngularFactor(btVector3(rotacionarX, rotacionarY, rotacionarZ));
                bt_obj_rb->setRestitution(elasticidade);
                bt_obj_rb->setGravity(btVector3(0, 0, 0));
                bt_obj_rb->setFriction(atrito);
                bt_obj_rb->setActivationState(DISABLE_DEACTIVATION);
                bt_obj_rb->setCollisionFlags(bt_obj_rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                dynamicsWorld->addRigidBody(bt_obj_rb);
                bt_obj = bt_obj_rb;
                bt_obj_rb->setGravity(btVector3(gravidade.x * gravity_force, gravidade.y * gravity_force, gravidade.z * gravity_force));
            }
        }

        collisionObject_obj.insert(pair<btCollisionObject *, shared_ptr<objeto_jogo>>(bt_obj, esse_objeto));
    }

    void aply_gravity()
    {
        if (bt_obj_rb != NULL && dinamica == dinamico)
        {
            bt_obj_rb->applyCentralForce(btVector3(gravidade.x * gravity_force * densidade, gravidade.y * gravity_force * densidade, gravidade.z * gravity_force * densidade));
        }
    }

    void atualisar()
    {
        // aply_gravity();
        shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
        if (tf != NULL && bt_obj != NULL && !gatilho)
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
                bu_collisions_no_per_object[esse_objeto.get()].clear();
            }
            bu_collisions_no_per_object.erase(esse_objeto.get());
        }

        if (bt_obj != NULL)
        {
            ///*
            mesh_shape_address = NULL;
            if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
            {
                dynamicsWorld->removeRigidBody((btRigidBody *)bt_obj);
            }
            else
            {
                dynamicsWorld->removeCollisionObject(bt_obj);
            }
            //*/
            if (compound != NULL)
            {
                delete compound;
            }

            deleteCollisionObject(bt_obj);
            if (collisionObject_obj.find(bt_obj) != collisionObject_obj.end())
            {
                collisionObject_obj.erase(bt_obj);
            }
            bt_obj = NULL;
            bt_obj_rb = NULL;
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
        if (bt_obj != NULL && bt_obj_rb != NULL && bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            bt_obj_rb->getMotionState()->setWorldTransform(tf);
            bt_obj->setWorldTransform(tf);
        }
        else
        {
            if (bt_obj != NULL)
            {
                bt_obj->setWorldTransform(tf);
            }
        }
    }
    void mudar_rot(quat rot)
    {
        btTransform tf = getObjectTransform(bt_obj);
        tf.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->getMotionState()->setWorldTransform(tf);
                bt_obj->setWorldTransform(tf);
            }
        }
        else
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj->setWorldTransform(tf);
            }
        }
    }
    void mudar_rot(vec3 rot)
    {
        mudar_rot(graus_quat(rot));
    }
    void adicionar_forca(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyCentralForce(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void adicionar_impulso(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyCentralImpulse(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void adicionar_velocidade(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->setLinearVelocity(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }

    void adicionar_forca_rotativo(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyTorque(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void adicionar_impulso_rotativo(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyTorqueImpulse(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void aplicar_velocidade_rotativa(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->setAngularVelocity(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    ~bullet()
    {
    }
};
/*
void bake_navmesh_3D()
{
    std::vector<std::shared_ptr<malha>> listaMeshes;
    std::vector<glm::mat4> listTransforms;
    for (pair<btCollisionObject *, shared_ptr<objeto_jogo>> p : collisionObject_obj)
    {

        shared_ptr<transform_> tf = p.second->pegar_componente<transform_>();
        shared_ptr<bullet> bu = p.second->pegar_componente<bullet>();

        if (tf && bu && !bu->gatilho && bu->dinamica == estatico && bu->collision_mesh)
        {

            listaMeshes.push_back(bu->collision_mesh);
            listTransforms.push_back(tf->pegar_matriz());
        }
    }

    if (navMesh)
    {
        dtFreeNavMesh(navMesh);
    }
    navMesh = gerarNavMesh(listaMeshes, listTransforms);
}
*/

class CustomContactResultCallback : public btCollisionWorld::ContactResultCallback
{
public:
    CustomContactResultCallback()
    {
    }

    virtual btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1)
    {
        colis_info collisionInfo;

        collisionInfo.pos = btToGlm(cp.getPositionWorldOnA());
        collisionInfo.nor = btToGlm(cp.m_normalWorldOnB);
        collisionInfo.velocidade = cp.m_appliedImpulse;

        objeto_jogo *A = collisionObject_obj[const_cast<btCollisionObject *>(colObj0Wrap->getCollisionObject())].get();
        objeto_jogo *B = collisionObject_obj[const_cast<btCollisionObject *>(colObj1Wrap->getCollisionObject())].get();
        shared_ptr<bullet> bu = A->pegar_componente<bullet>();

        if (bu->get_collision_info)
        {
            collisionInfo.obj = A;
            collisionInfo.cos_obj = B;
            physics_3D_collisionInfos.emplace_back(collisionInfo);
            bu->colis_infos.emplace_back(collisionInfo);
        }

        return 0; // return 0 to process all collisions
    }
};

class CustomOverlapFilterCallback : public btOverlapFilterCallback
{
public:
    virtual bool needBroadphaseCollision(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) const
    {
        // Obtenha os objetos associados aos proxies
        btCollisionObject *obj0 = static_cast<btCollisionObject *>(proxy0->m_clientObject);
        btCollisionObject *obj1 = static_cast<btCollisionObject *>(proxy1->m_clientObject);

        // Verifique se ambos os objetos são do tipo SensorTrigger (ou seja, não devem colidir entre si)
        bool isSensorTrigger0 = obj0->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;
        bool isSensorTrigger1 = obj1->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;

        // Evite a colisão entre objetos SensorTrigger
        if (isSensorTrigger0 && isSensorTrigger1)
        {
            return false;
        }

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
        result.distancia = glm::distance(rayFrom, result.pos);
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

        dynamicsWorld->setGravity(btVector3(0, 0, 0));

        global_bullet_iniciado++;
    }
}

thread bullet_starter;
void iniciar_iniciar_global_bullet()
{

    thread t(iniciar_global_bullet);
    bullet_threads.push_back(std::move(t));
}

void terminar_iniciar_global_bullet()
{

    for (std::thread &thread : bullet_threads)
    {
        thread.join();
    }
    bullet_threads.clear();
}

CustomContactResultCallback bullet_callback;
void get_3D_collisions()
{
    btCollisionObjectArray &bcoa = dynamicsWorld->getCollisionObjectArray();

    for (int i = 0; i < bcoa.size(); i++)
    {
        dynamicsWorld->contactTest(bcoa[i], bullet_callback);
    }
}

void clean_collisions()
{
    for (auto p : collisionObject_obj)
    {
        // p.second->pegar_componente<bullet>()->objects_touching = 0;
        p.second->pegar_componente<bullet>()->colis_infos.clear();
        p.second->pegar_componente<bullet>()->objs_touching.clear();
    }
    physics_3D_collisionInfos.clear();
}

void clean_bu_collisions_no_per_object()
{
    for (pair<objeto_jogo *, std::vector<objeto_jogo *>> p : bu_collisions_no_per_object)
    {
        bu_collisions_no_per_object[p.first].clear();
    }
}

void get_bu_collisions_no_per_object()
{
    for (colis_info ci : physics_3D_collisionInfos)
    {
        bu_collisions_no_per_object[(objeto_jogo *)ci.obj].push_back((objeto_jogo *)ci.cos_obj);
    }
}

void atualisar_global_bullet()
{

    clean_collisions();
    get_3D_collisions();
    clean_bu_collisions_no_per_object();
    get_bu_collisions_no_per_object();
    float bullet_passo_tempo = Tempo::varTempRender * Tempo::velocidadeTempo;
    if (Tempo::velocidadeTempo > 0)
    {
        dynamicsWorld->stepSimulation(bullet_passo_tempo, 8);
        //dynamicsWorld->stepSimulation(bullet_passo_tempo);
    }
}

void iniciar_atualisar_global_bullet()
{

    thread t(atualisar_global_bullet);
    bullet_threads.push_back(std::move(t));
}

void terminar_atualisar_global_bullet()
{

    for (std::thread &thread : bullet_threads)
    {
        thread.join();
    }
    bullet_threads.clear();
}