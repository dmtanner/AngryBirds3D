#include "target.h"

Target::Target() {

    targetMass = 1;

    targetDimX = 5;
    targetDimY = 5;
    targetDimZ = 5;

    targetLocX = 0;
    targetLocY = 0;
    targetLocZ = 0;

    initializeBullet();

}

Target::Target(float mass, btVector3 dimensions, btVector3 initLoc)
{
    targetMass = mass;

    targetDimX = dimensions.getX();
    targetDimY = dimensions.getY();
    targetDimZ = dimensions.getZ();

    targetLocX = initLoc.getX();
    targetLocY = initLoc.getY();
    targetLocZ = initLoc.getZ();

    initializeBullet();
}

Target::~Target()
{
    delete targetMotionState;
    delete targetRigidBody;
    delete targetShape;


}

void Target::initializeBullet()
{

    targetShape = new btBoxShape(btVector3(targetDimX, targetDimY, targetDimZ));

    //set box initial location at 50
    targetMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(targetLocX, targetLocY, targetLocZ)));

    //set box initial inertia
    btVector3 targetInertia;
    targetShape->calculateLocalInertia(targetMass, targetInertia);

    btRigidBody::btRigidBodyConstructionInfo targetRigidBodyCI(targetMass, targetMotionState, targetShape, targetInertia);
    targetRigidBody = new btRigidBody(targetRigidBodyCI);

    targetRigidBody->activate(true);

}

btRigidBody *Target::getRigidBody()
{
    return targetRigidBody;
}

void Target::getOpenGLMatrix(btScalar *m)
{
    btTransform trans = targetRigidBody->getWorldTransform();
    trans.getOpenGLMatrix(m);
}



