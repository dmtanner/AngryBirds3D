#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    world = new World();
    spacePressed = false;

    ui->setupUi(this);
    ui->widget->addWorld(world);
    startTimer(30);

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
            if(!spacePressed) {
                world->shoot();
            }
            spacePressed = true;
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
    std::stringstream score;
    score << "Score: " << world->getScore();
    QString qscore = QString::fromStdString(score.str());
    ui->scoreLabel->clear();
    ui->scoreLabel->repaint();
    ui->scoreLabel->setText(qscore);
}
