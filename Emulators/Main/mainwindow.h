#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controllerinput.h"
#include "viewmodel.h"
#include "mocmodel.h"

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
    ViewModel *mModel;
    void updateRomList();

private slots:
    void onControllerInput(Input::Keys keys);
    void onCurrentRowChanged(int currentRow);
};

#endif // MAINWINDOW_H
