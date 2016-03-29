#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  mUi(new Ui::MainWindow)
{
  mModel = new ViewModel();
  mocModel* moc = new mocModel(mModel);

  mUi->setupUi(this);
  mInput = ControllerInput::getInstance();

  int count = mModel->getEmulatorCount();
  for (int i = 0; i < count; ++i)
  {
    const Emulator* emu = mModel->getEmulatorForIndex(i);
    if (!emu->valid)
    {
      continue;
    }
    mUi->mListEmulators->addItem(QString::fromStdString(emu->Name));
  }

  // connect signals
  connect(mInput, SIGNAL(keyPressed(Input::Keys)), this, SLOT(onControllerInput(Input::Keys)));
  connect(mUi->mListEmulators, SIGNAL(currentRowChanged(int)), this, SLOT(onCurrentRowChanged(int)));

  // init focus
  mUi->mListEmulators->setFocus();
  mUi->mListEmulators->setCurrentRow(0);
  mUi->mListRoms->setCurrentRow(0);
}

MainWindow::~MainWindow()
{
  disconnect(mInput, SIGNAL(keyPressed(Input::Keys)), this, SLOT(onControllerInput(Input::Keys)));
  disconnect(mUi->mListEmulators, SIGNAL(currentRowChanged(int)), this, SLOT(onCurrentRowChanged(int)));
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

void MainWindow::onCurrentRowChanged(int currentRow)
{
  updateRomList();
}

void MainWindow::updateRomList()
{
  mUi->mListRoms->clear();
  int index = mUi->mListEmulators->currentRow();
  const Emulator* emu = mModel->getEmulatorForIndex(index);
  if (!emu->valid)
  {
    return;
  }
  const std::vector<const ROM*>* roms = mModel->getRomsForEmulator(emu);
  for(std::vector<const ROM*>::const_iterator it = roms->begin(); it != roms->end(); ++it)
  {
   this->mUi->mListRoms->addItem(QString::fromStdString((*it)->Name));
  }
}
