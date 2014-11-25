#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    world = new World();
    ui->setupUi(this);
    ui->widget->addWorld(world);
    //ui->widget->setWindowState(Qt::WindowFullScreen);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Left:
            world->toggleLeft();
            break;
        case Qt::Key_Right:
            world->toggleRight();
            break;
        case Qt::Key_Up:
            world->toggleUp();
            break;
        case Qt::Key_Down:
            world->toggleDown();
            break;
        case Qt::Key_Space:
            world->shoot();
            break;
        case Qt::Key_Escape:
            exit(0);
            break;

       default:
          break;
    }
    e->accept();  // Don't pass any key events to parent
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Left:
            world->toggleLeft();
            break;
        case Qt::Key_Right:
            world->toggleRight();
            break;
        case Qt::Key_Up:
            world->toggleUp();
            break;
        case Qt::Key_Down:
            world->toggleDown();
            break;

       default:
          break;
    }
    e->accept();  // Don't pass any key events to parent
}
