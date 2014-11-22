#include "world.h"

World::World() {
    gravity = 10;
    hp = 100;
    cannonBallRadius = 2.5;
    time_step = 1/60.0;     //amount of time that's passed in the real world
    max_sub_steps = 10;     //max number of internal steps bullet can take each time you call it
    fixed_time_step = 1/60.0;   //amount of time between bullet computations

    projectileMatrix = new btScalar[15];
    targetMatrix = new btScalar[15];

    initializeBullet();

    createTargets();
}

World::~World()
{
    //delete matrices
    delete projectileMatrix;
    delete targetMatrix;

    //delete cannonball
    dynamicsWorld->removeRigidBody(cannonBall->getRigidBody());

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

    //create new projectile
    float pMass = 1;
    float pRadius = 5;
    btVector3 initLoc(0, 50, 0);
    btVector3 initInertia(0, 50, 0);
    cannonBall = new Projectile(pMass, pRadius, initLoc, initInertia);


    //add projectile to world
    dynamicsWorld->addRigidBody(cannonBall->getRigidBody());


}

void World::step()
{
    dynamicsWorld->stepSimulation(time_step, max_sub_steps, fixed_time_step);

//    btTransform trans;
//    cannonBall->getRigidBody()->getMotionState()->getWorldTransform(trans);
//    std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;

//    projectileMatrix = new btScalar[15];
//    targetMatrix = new btScalar[15];
//    cannonBall->getOpenGLMatrix(projectileMatrix);
//    box->getOpenGLMatrix(targetMatrix);

}

btScalar *World::getProjectileMatrix()
{
    return projectileMatrix;
}

btScalar *World::getTargetMatrix()
{
    return targetMatrix;
}

std::vector<Target *> World::getTargets()
{
    return targets;
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
        btVector3 dimensions(1, 1, 1);
        btVector3 initLoc(x, 20, z);
        Target* t = new Target(tMass, dimensions, initLoc);

        //add to world
        dynamicsWorld->addRigidBody(t->getRigidBody());
        targets.push_back(t);
    }

}


