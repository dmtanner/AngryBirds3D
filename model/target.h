#ifndef TARGET_H
#define TARGET_H

#include <btBulletDynamicsCommon.h>

class Target {

public:
    Target();
    Target(float mass, btVector3 dimensions, btVector3 initLoc);  //sets mass, size and initial location
    ~Target();

    void initializeBullet();
    btRigidBody* getRigidBody();
    void getOpenGLMatrix(btScalar *m);

private:

    // bullet things
    btCollisionShape* targetShape;
    btDefaultMotionState* targetMotionState;
    btRigidBody* targetRigidBody;

    //my variables;
    float targetLocX;   //initial location
    float targetLocY;
    float targetLocZ;
    float targetDimX;   //dimensions
    float targetDimY;
    float targetDimZ;
    btScalar targetMass;    //mass

};

#endif // TARGET_H
