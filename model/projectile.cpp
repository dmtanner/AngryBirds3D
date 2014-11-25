#include "projectile.h"

Projectile::Projectile(){

    projectileMass = 1;
    projectileRadius = 5;

    projectileLocX = 0;
    projectileLocY = 0;
    projectileLocZ = 0;

    initializeBullet();

}

Projectile::Projectile(float mass, float radius, btVector3 initLoc)
{
    projectileMass = mass;
    projectileRadius = radius;

    projectileLocX = initLoc.getX();
    projectileLocY = initLoc.getY();
    projectileLocZ = initLoc.getZ();

    initializeBullet();

}

Projectile::~Projectile()
{
//    delete projectileRigidBody->getMotionState();
    delete projectileMotionState;
    delete projectileRigidBody;
    delete projectileShape;

}

void Projectile::initializeBullet()
{
    //create sphere with radius
    projectileShape = new btSphereShape(projectileRadius);

    //set ball initial location
    projectileMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(projectileLocX, projectileLocY, projectileLocZ)));

    //initialize everything
    btVector3 projectileInertia;
    projectileShape->calculateLocalInertia(projectileMass, projectileInertia);

    btRigidBody::btRigidBodyConstructionInfo projectileRigidBodyCI(projectileMass, projectileMotionState, projectileShape, projectileInertia);
    projectileRigidBody = new btRigidBody(projectileRigidBodyCI);

    projectileRigidBody->activate(true);
    projectileRigidBody->applyCentralImpulse(btVector3(1, 10, 3));

}

btRigidBody *Projectile::getRigidBody()
{
    return projectileRigidBody;
}

void Projectile::getOpenGLMatrix(btScalar *m)
{
    btTransform trans = projectileRigidBody->getWorldTransform();
    trans.getOpenGLMatrix(m);
}

void Projectile::applyImpulse(float x, float y, float z)
{
    projectileRigidBody->activate(true);
    projectileRigidBody->applyCentralImpulse(btVector3(x, y, z));
}

