#include "world.h"

World::World() {
    gravity = 15;
    hp = 100;
    cannonBallRadius = 2.5;
    cannonAngle = 45;
    viewAngle = 0;

    time_step = 1/60.0;     //amount of time that's passed in the real world
    max_sub_steps = 10;     //max number of internal steps bullet can take each time you call it
    fixed_time_step = 1/60.0;   //amount of time between bullet computations

    initializeBullet();

    createTargets();
}

World::~World()
{
    //delete ground
    dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundMotionState;
    delete groundRigidBody;
    delete groundShape;

    //delete everything else
    delete dynamicsWorld;
    delete solver;
    delete collisionConfig;
    delete collisionDispatcher;
    delete broadphase;

}

void World::initializeBullet()
{
    //initialize bullet necessities
    broadphase = new btDbvtBroadphase();
    collisionConfig = new btDefaultCollisionConfiguration();
    collisionDispatcher = new btCollisionDispatcher(collisionConfig);
    solver = new btSequentialImpulseConstraintSolver();

    //create world
    dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, solver, collisionConfig);
    dynamicsWorld->setGravity(btVector3(0, -gravity, 0));

    //create ground
    groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    groundRigidBody = new btRigidBody(groundRigidBodyCI);

    //add ground to world
    dynamicsWorld->addRigidBody(groundRigidBody);


}

void World::step()
{
    dynamicsWorld->stepSimulation(time_step, max_sub_steps, fixed_time_step);
}

std::vector<Target *> World::getTargets()
{
    return targets;
}

std::vector<Projectile *> World::getProjectiles()
{
    return projectiles;
}

void World::incrementCannonAngle(float increment)
{
    cannonAngle += increment;

    //make sure cannon is pointed within range
    if(cannonAngle > 70)
        cannonAngle = 70;
    if(cannonAngle < 0)
        cannonAngle = 0;
}

float World::getCannonAngle()
{
    return cannonAngle;
}

void World::incrementViewAngle(float increment)
{
    viewAngle += increment;
}

float World::getViewAngle()
{
    return viewAngle;
}

void World::shoot()
{
    float mass = 3;
    float radius = 0.5;
    float x = cos(viewAngle);
    float z = sin(viewAngle);
    float y = 1;
    btVector3 initLoc(0, 0, 0);
    Projectile* p = new Projectile(mass, radius, initLoc);

    dynamicsWorld->addRigidBody(p->getRigidBody());
    projectiles.push_back(p);

    p->applyImpulse(50*x, 5, 50*z);
}

void World::createTargets()
{
    //create on 20x20 grid
    int upperBound = 10;
    int lowerBound = -10;
    int range = upperBound - lowerBound;

    for(int i = 0; i < targetCount; i++) {

        float x = (((float)rand() / RAND_MAX) * range) + lowerBound;
        float z = (((float)rand() / RAND_MAX) * range) + lowerBound;

        float tMass = 3;
        btVector3 dimensions(0.5, 0.5, 0.5);
        btVector3 initLoc(x, 0.5, z);
        Target* t = new Target(tMass, dimensions, initLoc);

        //add to world
        dynamicsWorld->addRigidBody(t->getRigidBody());
        targets.push_back(t);
    }

}


