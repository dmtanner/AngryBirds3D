#ifndef WORLD_H
#define WORLD_H

//#include bullet
#include <btBulletDynamicsCommon.h>

#include <vector>
#include <iostream>
#include "target.h"
#include "projectile.h"
#include <sstream>
#include <cstdlib>

#define PI 3.14159265359


class World
{
public:
    World();
    ~World();

    void initializeBullet();
    void step();

    std::vector<Target *> getTargets();
    std::vector<Projectile *> getProjectiles();

    void incrementCannonAngle(float increment);
    float getCannonAngle();
    void incrementViewAngle(float increment);
    float getViewAngle();

    void shoot();

    void toggleLeft();
    void toggleRight();
    void toggleUp();
    void toggleDown();


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


    //non-bullet things
    float hp;
    float gravity;
    float cannonBallRadius;
    float cannonAngle;
    float viewAngle;

    btVector3 viewLocation = btVector3(0, 5, 0);    //unused yet

    bool leftPress;
    bool rightPress;
    bool upPress;
    bool downPress;

    std::vector<Target*> targets;
    std::vector<Projectile*> projectiles;
    float time_step;
    float max_sub_steps;
    float fixed_time_step;

    static const int targetCount = 30;


    void createTargets();
    void addRigidBody();

};

#endif // WORLD_H
