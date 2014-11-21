#ifndef WORLD_H
#define WORLD_H

//#include bullet
#include <btBulletDynamicsCommon.h>

#include <vector>
#include <iostream>
#include "target.h"
#include "cannon.h"
#include "projectile.h"
#include <sstream>
#include <cstdlib>


class World
{
public:
    World();
    ~World();

    void initializeBullet();
    void step();
    btScalar* getProjectileMatrix();
    btScalar* getTargetMatrix();

    std::vector<Target *> getTargets();

private:
    // bullet things
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicsWorld;

    btCollisionShape* groundShape;

    btDefaultMotionState* groundMotionState;

    btRigidBody* groundRigidBody;

    btScalar *projectileMatrix;
    btScalar *targetMatrix;

    //my objects
    Projectile* cannonBall;
    Target* box;


    //non-bullet things
    float hp;
    float gravity;
    float cannonBallRadius;
    std::vector<Target*> targets;
    std::vector<Projectile*> projectiles;
    Cannon cannon;
    float time_step;
    float max_sub_steps;
    float fixed_time_step;

    int targetCount = 30;


    void createTargets();
    void addRigidBody();

};

#endif // WORLD_H
