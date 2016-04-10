#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <Windows.h>
#include "controllerinput.h"
#include "viewmodel.h"
#include "mocmodel.h"
#include "loader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( char *argv[], QWidget *parent = 0);
    ~MainWindow();

protected:
   void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *mUi;
    ControllerInput *mInput;
    ViewModel *mModel;
    void updateRomList();

private slots:
    void onControllerInput(Input::Keys keys);
    void onCurrentRowChanged(int currentRow);
    void onRomListItemDoubleClicked(QListWidgetItem* item);
};

#endif // MAINWINDOW_H
