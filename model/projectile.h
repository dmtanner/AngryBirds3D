#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <QVector3D>

class Projectile {

public:
    Projectile();
    Projectile(float mass, float radius, btVector3 initLoc, QVector3D color);
    ~Projectile();


    btRigidBody* getRigidBody();
    QVector3D getColor();
    float getRadius();
    void getOpenGLMatrix(btScalar *m);
    void applyImpulse(float x, float y, float z);

private:

    // bullet things
    void initializeBullet();

    btCollisionShape* projectileShape;
    btDefaultMotionState* projectileMotionState;

    btRigidBody* projectileRigidBody;


    //my variables;
    btScalar projectileRadius;
    btScalar projectileMass;

    QVector3D color;

    float projectileLocX;   //initial location
    float projectileLocY;
    float projectileLocZ;

};

#endif // PROJECTILE_H
