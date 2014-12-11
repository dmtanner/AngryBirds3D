#include "world.h"

World::World() {
    score = 0;
    gravity = 25;
    hp = 100;
    cannonBallRadius = 2.5;
    cannonAngle = 45;
    cannonPower = 70;
    viewAngle = 0;
    turnSpeed = 2;

    leftPress = false;
    rightPress = false;
    upPress = false;
    downPress = false;

    time_step = 1/60.0;     //amount of time that's passed in the real world
    max_sub_steps = 10;     //max number of internal steps bullet can take each time you call it
    fixed_time_step = 1/60.0;   //amount of time between bullet computations

    initializeBullet();

    createTargets();

    setMaxScore();
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

void World::calculateScore()
{
    //sum up heights for score
    score = 0;
    for(std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
        int height = (*it)->getRigidBody()->getWorldTransform().getOrigin().getY();
        score += height;
    }

    score = maxScore - score;

    score = (score/(float)maxScore)*MAX_SCORE;
}

void World::setMaxScore()
{
    maxScore = 0;
    for(std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
        int height = (*it)->getRigidBody()->getWorldTransform().getOrigin().getY();
        maxScore += height;
    }
}

void World::step()
{
    dynamicsWorld->stepSimulation(time_step, max_sub_steps, fixed_time_step);

    calculateScore();

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

std::vector<Enemy *> World::getEnemies()
{
    return enemies;
}

void World::incrementCannonAngle(float increment)
{
    cannonAngle += increment;

    //make sure cannon is pointed within range
    if(cannonAngle > 55)
        cannonAngle = 55;
    if(cannonAngle < 5)
        cannonAngle = 5;
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
    float radius = 0.3;
    QVector3D color = QVector3D(0.0, 0.0, 0.0);

    //coordinates at end of cannon - similar to sphere coordinates algorithm
    float x = cos(viewAngle) * cos(cannonAngle*PI/180);
    float z = sin(viewAngle) * cos(cannonAngle*PI/180);
    float y = sin(cannonAngle*PI/180);

    btVector3 initLoc(x, y, z);
    initLoc = (2 - radius) * initLoc;
    Projectile* p = new Projectile(mass, radius, initLoc, color);

    dynamicsWorld->addRigidBody(p->getRigidBody());
    projectiles.push_back(p);

    p->applyImpulse(cannonPower*x, cannonPower*y, cannonPower*z);

    //limit number of projectiles
    if(projectiles.size() > MAX_PROJECTILES) {
        //delete projectile at beginning of list
        dynamicsWorld->removeRigidBody(projectiles.front()->getRigidBody());
        projectiles.erase(projectiles.begin());
    }
}

void World::setCannonPower(int power)
{
    cannonPower = power;
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
    //create targets on a bounded grid
    int upperBound = 50;
    int lowerBound = -50;
    int range = upperBound - lowerBound;

    for(int i = 0; i < targetCount; i++) {

        float x = (((float)rand() / RAND_MAX) * range) + lowerBound;
        float z = (((float)rand() / RAND_MAX) * range) + lowerBound;

        //protect viewer from too-close targets
        if(x < 5 && x > -5)
            x += 10*((float)rand()/RAND_MAX);
        if(z < 5 && z > -5)
            z += 10*(float)rand()/RAND_MAX;

        /*-------used for an ordered, circular placement------*/
        //x = targetCount*cos(2*3.14159*i/targetCount);
        //z = targetCount*sin(2*3.14159*i/targetCount);

        createPillar(btVector3(x,0.0,z));
    }

}

void World::createPillar(btVector3 location)
{
    //bottom
    float mass = 3;
    btVector3 bottomDim(0.5, 1.5, 0.5);
    location.setY(location.getY() + bottomDim.getY());
    Target* bottom = new Target(mass, bottomDim, location);
    dynamicsWorld->addRigidBody(bottom->getRigidBody());
    targets.push_back(bottom);

    //middle
    btVector3 middleDim(1.5, 0.5, 1.5);
    location.setY(location.getY()*2 + middleDim.getY());
    Target* middle = new Target(mass, middleDim, location);
    dynamicsWorld->addRigidBody(middle->getRigidBody());
    targets.push_back(middle);

    //side1
    btVector3 sideDim(0.5, 1.0, 0.5);
    btVector3 location1 = location;
    location1.setY(location1.getY() + middleDim.getY() + sideDim.getY());
    location1.setX(location1.getX() + middleDim.getX()/2);
    location1.setZ(location1.getZ() + middleDim.getZ()/2);
    Target* side1 = new Target(mass, sideDim, location1);
    dynamicsWorld->addRigidBody(side1->getRigidBody());
    targets.push_back(side1);

    //side2
    btVector3 location2 = location;
    location2.setY(location2.getY() + middleDim.getY() + sideDim.getY());
    location2.setX(location2.getX() - middleDim.getX()/2);
    location2.setZ(location2.getZ() - middleDim.getZ()/2);
    Target* side2 = new Target(mass, sideDim, location2);
    dynamicsWorld->addRigidBody(side2->getRigidBody());
    targets.push_back(side2);

    location.setY(location1.getY());

    //top
    btVector3 topDim(1.5, 1.5, 1.5);
    location.setY(location.getY() + sideDim.getY() + topDim.getY());
    Target* top = new Target(mass, topDim, location);
    dynamicsWorld->addRigidBody(top->getRigidBody());
    targets.push_back(top);

    //enemy on top
    float enemyDim = 0.5;
    location.setY(location.getY() + topDim.getY() + enemyDim);
    QVector3D color = QVector3D(0, 100, 150);
    Enemy* e = new Enemy(mass, enemyDim, location, color);
    dynamicsWorld->addRigidBody(e->getRigidBody());
    enemies.push_back(e);

}


