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
       case Qt::Key_Left:  /*  Escape Key */
            std::cout << "left" << std::endl;
          //degrees += 2;
          break;
       case Qt::Key_Right: // reset physics to time 0
       {
        //degrees -= 2;
         break;
       }
    case Qt::Key_Up:
    {
        world->incrementCannonAngle(1);
        break;
    }
    case Qt::Key_Down:
    {
        world->incrementCannonAngle(-1);
        break;
    }
    case Qt::Key_Escape:
        {
            exit(0);
            break;
        }

       default:
          break;
    }
    e->accept();  // Don't pass any key events to parent
}
