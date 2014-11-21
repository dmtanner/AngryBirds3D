#include "widget.h"

Widget::Widget(QWidget *parent) :
    QGLWidget(parent)
{
    startTimer(30);
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

    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    push();
        //change with user input
        float xdir = 0.5;
        float zdir = 0.5;
        //modelview.lookAt(QVector3D(0, 0, 0), QVector3D(xdir, 0, zdir), QVector3D(0, 1, 0));
        modelview.lookAt(QVector3D(2, 3, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
        //drawCylinder();
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

void Widget::timerEvent(QTimerEvent *)
{
    //world->step();
    updateGL();
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

}

void Widget::drawTargets()
{
    std::vector<Target*> targets = world->getTargets();
    for(std::vector<Target*>::iterator it = targets.begin(); it != targets.end(); it++) {
        Target* t = *it;
        btScalar* targetMatrix = new btScalar[15];
        t->getOpenGLMatrix(targetMatrix);
        push();
            drawCube();
            projection.copyDataTo(targetMatrix);
        pop();
    }
}

void Widget::drawProjectiles()
{

}


