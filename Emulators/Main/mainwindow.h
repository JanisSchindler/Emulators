#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "..\Input\controllerinput.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *mUi;
    ControllerInput *mInput;
};

#endif // MAINWINDOW_H
