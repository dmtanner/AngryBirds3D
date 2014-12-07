#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    world = new World();
    spacePressed = false;
    shootTimer = 0;

    ui->setupUi(this);
    ui->widget->addWorld(world);
    ui->cannonPowerSlider->setValue(70);
    startTimer(20);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete world;
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
            if(!spacePressed) {
                //world->shoot();
            }
            spacePressed = true;
            break;
        case Qt::Key_Comma: //left angle bracket
            ui->cannonPowerSlider->setValue(ui->cannonPowerSlider->value() - 1);
            break;
        case Qt::Key_Period: //left angle bracket
            ui->cannonPowerSlider->setValue(ui->cannonPowerSlider->value() + 1);
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
        case Qt::Key_Space:
            spacePressed = false;
            break;

       default:
          break;
    }
    e->accept();  // Don't pass any key events to parent
}

void MainWindow::timerEvent(QTimerEvent *)
{
    //Display Score
    std::stringstream score;
    score << "Score: " << world->getScore();
    QString qscore = QString::fromStdString(score.str());

    ui->scoreLCD->display(world->getScore());

    //shoot
    if(spacePressed && shootTimer > 15) {
        world->shoot();
        shootTimer = 0;
    }
    shootTimer += 1;

    //Update model and view
    world->step();
    ui->widget->updateGL();

}

void MainWindow::on_cannonPowerSlider_valueChanged(int value)
{
    world->setCannonPower(value);
}
