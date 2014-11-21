#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <btBulletDynamicsCommon.h>
#include <iostream>

class Projectile {

public:
    Projectile();
    Projectile(float mass, float radius, btVector3 initLoc, btVector3 initInertia);
    ~Projectile();

    void initializeBullet();
    btRigidBody* getRigidBody();
    void getOpenGLMatrix(btScalar *m);

private:

    // bullet things
    btCollisionShape* projectileShape;
    btDefaultMotionState* projectileMotionState;

    btRigidBody* projectileRigidBody;


    //my variables;
    btScalar projectileRadius;
    btScalar projectileMass;

    float projectileLocX;   //initial location
    float projectileLocY;
    float projectileLocZ;

    float projectileInertiaX;
    float projectileInertiaY;
    float projectileInertiaZ;

};

#endif // PROJECTILE_H
