#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QKeyEvent>

#include "model/world.h"

#define PI 3.14159265359

class Widget : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void addWorld(World* w);

private:

    /*-------Model Interface Code------*/
    World* world;


    /*-----OpenGL Items----------*/
    QMatrix4x4 projection;
    QMatrix4x4 modelview;
    std::vector<QMatrix4x4> modelviewStack;
    QOpenGLShaderProgram program;

    QVector3D red = QVector3D(1.0, 0.0, 0.0);
    QVector3D green = QVector3D(0.0, 1.0, 0.0);
    QVector3D blue = QVector3D(0.0, 0.0, 1.0);


    /*-----OpenGL Drawing Functions----------*/
    void push();
    void pop();
    void drawCannon();
    void drawTargets();
    void drawProjectiles();
    float randomNum(int lowerBound, int upperBound);

    virtual void timerEvent(QTimerEvent *);


    /*-------OpenGL Object Data--------*/
    GLuint positionBuffer;
    GLuint colorBuffer;
    GLuint normBuffer;

    GLuint circlePositionBuffer;
    GLuint circleColorBuffer;
    GLuint circleNormBuffer;

    GLuint conePositionBuffer;
    GLuint coneColorBuffer;
    GLuint coneNormBuffer;

    GLuint cylinderPositionBuffer;
    GLuint cylinderColorBuffer;
    GLuint cylinderNormBuffer;

    GLuint spherePositionBuffer;
    GLuint sphereColorBuffer;
    GLuint sphereNormBuffer;
    GLuint sphereIndexBuffer;

    int indexSize;
    static const int SLICES = 30;
    static const float height = 1.0;


    /*------------Shader Program Variables-----------*/
    GLuint pvmMatrixUniform;
    GLuint positionAttribute;
    GLuint colorAttribute;
    GLuint normAttribute;


    /*-------------------Draw Functions-----------------*/
    //2D primitives
    void drawTriangle();
    void drawCircle();
    void drawSquare();

    //3D primitives
    void drawCube();
    void drawCone();
    void drawCylinder();
    void drawSphere();


    /*-------Initialiazation of OpenGL Buffers-------*/
    void initializeShader();
    void initializeBuffers();

    void initializeSquare();
    void initializeCircle();
    void initializeCone();
    void initializeCylinder();
    void initializeSphere();


    /*------Bind Buffers---------*/
    void bindTriangleAttributes();
    void bindCircleAttributes();
    void bindConeAttributes();
    void bindCylinderAttributes();
    void bindSphereAttributes();

signals:

public slots:

};

#endif // WIDGET_H
