#include <iostream>
#include <btBulletDynamicsCommon.h>

int main(int argc, char* argv[]) {
    // Set up the physics world
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    // Create the ground shape and its motion state
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);

    // Create the ball shape and its motion state
    btCollisionShape* ballShape = new btSphereShape(1);
    btDefaultMotionState* ballMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    btScalar ballMass = 1;
    btVector3 ballInertia(0, 0, 0);
    ballShape->calculateLocalInertia(ballMass, ballInertia);
    btRigidBody::btRigidBodyConstructionInfo ballRigidBodyCI(ballMass, ballMotionState, ballShape, ballInertia);
    btRigidBody* ballRigidBody = new btRigidBody(ballRigidBodyCI);
    dynamicsWorld->addRigidBody(ballRigidBody);

    // Simulation loop
    for (int i = 0; i < 300; i++) {
        dynamicsWorld->stepSimulation(1 / 60.f, 10);

        btTransform ballTransform;
        ballRigidBody->getMotionState()->getWorldTransform(ballTransform);
        btVector3 ballPosition = ballTransform.getOrigin();
        std::cout << "objects: " << dynamicsWorld->getNumCollisionObjects() << std::endl;
        std::cout << "Ball position: (" << ballPosition.getX() << ", " << ballPosition.getY() << ", " << ballPosition.getZ() << ")\n";

        if (ballPosition.getY() <= 1.001) { // Check if the ball has hit the floor (considering a small threshold for floating point errors)
            std::cout << "Hit the floor!\n";
            break;
        }
    }

    // Clean up
    dynamicsWorld->removeRigidBody(ballRigidBody);
    dynamicsWorld->removeRigidBody(groundRigidBody);

    delete ballRigidBody->getMotionState();
    delete ballRigidBody;
    delete ballShape;
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;
    delete groundShape;
}