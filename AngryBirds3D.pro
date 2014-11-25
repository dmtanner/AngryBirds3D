#-------------------------------------------------
#
# Project created by QtCreator 2014-11-21T10:50:08
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AngryBirds3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widget.cpp \
    model/projectile.cpp \
    model/target.cpp \
    model/world.cpp \
    widgetinit.cpp

HEADERS  += mainwindow.h \
    widget.h \
    model/projectile.h \
    model/target.h \
    model/world.h

INCLUDEPATH += /home/marcos/QTworkspace/Bullet/bullet-2.80-rev2531/src
LIBS += -L/home/marcos/QTworkspace/Bullet/bullet-2.80-rev2531/lib  \
	-lBulletDynamics  \
	-lBulletCollision \
	-lLinearMath \

FORMS    += mainwindow.ui

OTHER_FILES += \
    BirdVertShader.vsh \
    BirdFragShader.fsh
