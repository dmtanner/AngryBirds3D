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

    void step();
    int getScore();

    std::vector<Target *> getTargets();
    std::vector<Projectile *> getProjectiles();

    void incrementCannonAngle(float increment);
    float getCannonAngle();
    void incrementViewAngle(float increment);
    float getViewAngle();
    QVector3D getGroundColor();

    void shoot();
    void setCannonPower(int power);

    void toggleLeft();
    void toggleRight();
    void toggleUp();
    void toggleDown();


private:

    /*---------bullet things---------*/
    void initializeBullet();

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicsWorld;
    btCollisionShape* groundShape;
    btDefaultMotionState* groundMotionState;
    btRigidBody* groundRigidBody;

    /*------------my bullet things----------*/
    float time_step;
    float max_sub_steps;
    float fixed_time_step;


    /*-------------non-bullet things-------*/
    int score;
    float hp;
    float gravity;
    float cannonBallRadius;
    float cannonAngle;
    float cannonPower;
    float viewAngle;
    float turnSpeed;

    btVector3 viewLocation = btVector3(0, 5, 0);    //unused yet

    QVector3D groundColor = QVector3D(0.2, 0.7, 0.1);

    int calculateScore();


    /*--------------user input control------------*/
    bool leftPress;
    bool rightPress;
    bool upPress;
    bool downPress;

    std::vector<Target*> targets;
    std::vector<Projectile*> projectiles;


    static const int targetCount = 30;
    static const int MAX_PROJECTILES = 100;


    void createTargets();

};

#endif // WORLD_H
