#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model/world.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    World* world = new World();
    ui->setupUi(this);
    ui->widget->addWorld(world);
}

MainWindow::~MainWindow()
{
    delete ui;
}
