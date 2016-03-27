#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  mUi(new Ui::MainWindow)
{
  mModel = new ViewModel();

  mUi->setupUi(this);
  mInput = ControllerInput::getInstance();
  mUi->mListRoms->addItem("My Test");
  mUi->mListRoms->addItem("My Test2");
  mUi->mListRoms->addItem("My Test3");

  mUi->mListEmulators->addItem("foo1");
  mUi->mListEmulators->addItem("foo2");
  mUi->mListEmulators->addItem("foo3");

  // init focus
  mUi->mListEmulators->setFocus();
  mUi->mListEmulators->setCurrentRow(0);
  mUi->mListRoms->setCurrentRow(0);

  connect(mInput, SIGNAL(keyPressed(Input::Keys)), this, SLOT(onControllerInput(Input::Keys)));
}

MainWindow::~MainWindow()
{
  disconnect(mInput, SIGNAL(keyPressed(Input::Keys)), this, SLOT(onControllerInput(Input::Keys)));
  delete mUi;
  delete mInput;
  delete mModel;
}

void HandleNavigationKeys(Input::Keys keys, QListWidget* list)
{
  int curr = list->currentRow();
  if (keys & Input::Up && curr > 0)
  {
    list->setCurrentRow(curr-1);
  }
  else if (keys & Input::Down && curr < list->count() - 1)
  {
    list->setCurrentRow(curr+1);
  }
}

void MainWindow::onControllerInput(Input::Keys keys)
{
  if (keys == Input::None)
  {
    return;
  }
  int curr = 0;
  if (mUi->mListEmulators->hasFocus())
  {
    HandleNavigationKeys(keys, mUi->mListEmulators);
    if (keys & Input::Accept)
    {
       mUi->mListRoms->setFocus();
    }
    return;
  }
  else if (mUi->mListRoms->hasFocus())
  {
    HandleNavigationKeys(keys, mUi->mListRoms);
    if (keys & Input::Back)
    {
       mUi->mListEmulators->setFocus();
    }
  }
}

