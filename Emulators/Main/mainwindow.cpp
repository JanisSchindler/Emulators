#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);
    mInput = new ControllerInput();
}

MainWindow::~MainWindow()
{
    delete mUi;
    delete mInput;
}
