#include "projectile.h"

Projectile::Projectile(){

    projectileMass = 1;
    projectileRadius = 5;

    projectileLocX = 0;
    projectileLocY = 0;
    projectileLocZ = 0;

    initializeBullet();

}

Projectile::Projectile(float mass, float radius, btVector3 initLoc, btVector3 initInertia)
{
    projectileMass = mass;
    projectileRadius = radius;

    projectileLocX = initLoc.getX();
    projectileLocY = initLoc.getY();
    projectileLocZ = initLoc.getZ();

    projectileInertiaX = initInertia.getX();
    projectileInertiaY = initInertia.getY();
    projectileInertiaZ = initInertia.getZ();

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

    projectileShape = new btSphereShape(projectileRadius);

    //set ball initial location at 50
    projectileMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(projectileLocX, projectileLocY, projectileLocZ)));

    btVector3 projectileInertia(projectileInertiaX, projectileInertiaY, projectileInertiaZ);
    projectileShape->calculateLocalInertia(projectileMass, projectileInertia);

    projectileMass = 1;

    btRigidBody::btRigidBodyConstructionInfo projectileRigidBodyCI(projectileMass, projectileMotionState, projectileShape, projectileInertia);
    projectileRigidBody = new btRigidBody(projectileRigidBodyCI);

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

