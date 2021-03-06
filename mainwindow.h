    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <sstream>

#include "model/world.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_cannonPowerSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void timerEvent(QTimerEvent *);
    bool spacePressed;
    int shootTimer;

    //universal data
    World* world;
    int viewRotation;
};

#endif // MAINWINDOW_H
