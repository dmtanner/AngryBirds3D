#include "world.h"

World::World() {
    score = 0;
    gravity = 25;
    hp = 100;
    cannonBallRadius = 2.5;
    cannonAngle = 45;
    viewAngle = 0;
    turnSpeed = 3.5;

    leftPress = false;
    rightPress = false;
    upPress = false;
    downPress = false;

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



    //delete targets
    for(std::vector<Target*>::iterator it = targets.begin(); it != targets.end(); it++) {
        dynamicsWorld->removeRigidBody((*it)->getRigidBody());
        delete *it;
    }

    //delete projectiles
    for(std::vector<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); it++) {
        dynamicsWorld->removeRigidBody((*it)->getRigidBody());
        delete *it;
    }

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

    score += 1;

    if(leftPress) {
        incrementViewAngle(-0.01*turnSpeed);
    }
    else if(rightPress) {
        incrementViewAngle(0.01*turnSpeed);
    }

    if(upPress) {
        incrementCannonAngle(1);
    }
    else if(downPress) {
        incrementCannonAngle(-1);
    }
}

int World::getScore()
{
    return score;
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
    if(cannonAngle > 55)
        cannonAngle = 55;
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

QVector3D World::getGroundColor()
{
    return groundColor;
}

void World::shoot()
{
    float mass = 3;
    float radius = 0.5;

    //coordinates at end of cannon - similar to sphere coordinates algorithm
    float x = cos(viewAngle) * cos(cannonAngle*PI/180);
    float z = sin(viewAngle) * cos(cannonAngle*PI/180);
    float y = sin(cannonAngle*PI/180);

    btVector3 initLoc(x, y, z);
    initLoc = 2.2 * initLoc;
    Projectile* p = new Projectile(mass, radius, initLoc);

    dynamicsWorld->addRigidBody(p->getRigidBody());
    projectiles.push_back(p);

    p->applyImpulse(50*x, 50*y, 50*z);

    //limit number of projectiles
    if(projectiles.size() > MAX_PROJECTILES) {
        //delete projectile at beginning of list
        dynamicsWorld->removeRigidBody(projectiles.front()->getRigidBody());
        projectiles.erase(projectiles.begin());
    }
}

void World::toggleLeft()
{
    leftPress = !leftPress;
}

void World::toggleRight()
{
    rightPress = !rightPress;
}

void World::toggleUp()
{
    upPress = !upPress;
}

void World::toggleDown()
{
    downPress = !downPress;
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

        x = targetCount*cos(2*3.14159*i/targetCount);
        z = targetCount*sin(2*3.14159*i/targetCount);

        float tMass = 3;
        btVector3 dimensions(0.5, 0.5, 0.5);
        btVector3 initLoc(x, 0.5, z);
        Target* t = new Target(tMass, dimensions, initLoc);

        //add to world
        dynamicsWorld->addRigidBody(t->getRigidBody());
        targets.push_back(t);

        //add one on top
        for(int j = 0; j < 5; j++) {
            btVector3 initLoc2(x, j + 1.5, z);
            Target* t2 = new Target(tMass, dimensions, initLoc2);
            t2->getRigidBody()->setRestitution(btScalar(1.1));
            t2->getRigidBody()->setFriction(btScalar(1.0));
            dynamicsWorld->addRigidBody(t2->getRigidBody());
            targets.push_back(t2);
        }
    }

}


