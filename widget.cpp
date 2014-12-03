#include "widget.h"

Widget::Widget(QWidget *parent) :
    QGLWidget(parent)
{

}

Widget::~Widget()
{
    // be sure to delete all buffers from the GPU
    GLuint bufferNames[3];
    bufferNames[0] = colorBuffer;
    bufferNames[1] = positionBuffer;
    bufferNames[2] = normBuffer;
    glDeleteBuffers(3, bufferNames);
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();

    std::cout << glGetString(GL_VENDOR) << " ";
    std::cout << glGetString(GL_VERSION) << std::endl;

    initializeBuffers();
    initializeShader();

    glEnableVertexAttribArray(colorAttribute);
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(normAttribute);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.53, 0.81, 0.98, 0.0);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    push();
        //change view with user input
        float xdir = cos(world->getViewAngle());
        float zdir = sin(world->getViewAngle());
        modelview.lookAt(QVector3D(0, 3, 0), QVector3D(xdir, 2.5  , zdir), QVector3D(0, 1, 0));

        drawGround();
        drawCannon();
        drawTargets();
        drawProjectiles();
    pop();
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(90, w/(float)h, 0.1, 100);
}

void Widget::addWorld(World *w)
{
    world = w;
}

void Widget::push()
{
    modelviewStack.push_back(modelview);
}

void Widget::pop()
{
    modelview = modelviewStack.back();
    modelviewStack.pop_back();
}

void Widget::drawCannon()
{
    push();
        //draw cannon swivel ball
        modelview.scale(2.5, 2.5, 2.5);
        drawSphere();
    pop();
    push();
        //set cannon at appropriate angle
        modelview.rotate(90, 0, 1, 0);
        modelview.rotate((90 - world->getCannonAngle()), cos(world->getViewAngle()), 0, sin(world->getViewAngle()));
        modelview.scale(0.5, 2, 0.5);
        modelview.translate(0, 0.5, 0);
        drawCylinder();
    pop();
}

void Widget::drawTargets()
{
    std::vector<Target*> targets = world->getTargets();
    for(std::vector<Target*>::iterator it = targets.begin(); it != targets.end(); it++) {
        Target* t = *it;
        btScalar* targetMatrix = new btScalar[16];
        t->getOpenGLMatrix(targetMatrix);
        push();
            modelview = modelview * QMatrix4x4((GLfloat*)targetMatrix).transposed();
            drawCube(t->getColor());
        pop();

        delete targetMatrix;
    }

}

void Widget::drawProjectiles()
{
    std::vector<Projectile*> projectiles = world->getProjectiles();
    for(std::vector<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); it++) {
        Projectile* p = *it;
        btScalar* projectileMatrix = new btScalar[16];
        p->getOpenGLMatrix(projectileMatrix);
        push();
            modelview = modelview * QMatrix4x4((GLfloat*)projectileMatrix).transposed();
            drawSphere();
        pop();

        delete projectileMatrix;
    }
}

void Widget::drawGround()
{
    push();
        modelview.scale(1000.0, 0.0001, 1000.0);
        modelview.rotate(90, 1.0, 0.0, 0.0);
        drawSquare(world->getGroundColor());
    pop();
}


