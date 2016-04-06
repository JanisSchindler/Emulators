#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(char *argv[], QWidget *parent) :
  QMainWindow(parent),
  mUi(new Ui::MainWindow)
{
  mModel = new ViewModel();

  mUi->setupUi(this);
  mInput = ControllerInput::getInstance();

  Loader::Load(argv[0], mModel);
  if (mModel->getEmulatorCount() <= 0)
  {
    mUi->mListEmulators->addItem("Failed to load any emulator");
    return;
  }

  int count = mModel->getEmulatorCount();
  for (int i = 0; i < count; ++i)
  {
    const Emulator* emu = mModel->getEmulatorForIndex(i);
    if (!emu->valid)
    {
      continue;
    }
    mUi->mListEmulators->addItem(QString::fromStdString(emu->name));
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

void StartROM(const Emulator* emulator, const ROM* rom)
{
  LPCWSTR applicationName = emulator->executablePath;
  QString commandLine = QString::fromStdWString(emulator->arguments);

  // build command line -> replace the <rom> macro and put it in hyphens
  QString romPath('\"');
  romPath.append(QString::fromStdWString(rom->path));
  romPath.append("\"");
  commandLine.replace("<ROM>", romPath, Qt::CaseInsensitive);

  qDebug() << commandLine;

  int length = commandLine.length() + 1;

  LPWSTR wcharCmdLine = new wchar_t[length];

  commandLine.toWCharArray(wcharCmdLine);
  wcharCmdLine[length - 1] = '\0';
  // additional information
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  // set the size of the structures
  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  // start the program up
  // TODO: keep the handle so we can close it later

  CreateProcess(applicationName,   // the path
                wcharCmdLine,        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory
                &si,            // Pointer to STARTUPINFO structure
                &pi);          // Pointer to PROCESS_INFORMATION structure

  // Close process and thread handles.
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

void MainWindow::onControllerInput(Input::Keys keys)
{
  if (keys == Input::None)
  {
    return;
  }
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
    if (keys & Input::Accept)
    {
      const Emulator* emulator = mModel->getEmulatorForIndex(mUi->mListEmulators->currentRow());
      const ROM* rom = mModel->getRomForIndex(emulator, mUi->mListRoms->currentRow());
      StartROM(emulator, rom);
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
    this->mUi->mListRoms->addItem(QString::fromStdString((*it)->name));
  }
  this->mUi->mListRoms->setCurrentRow(0);
}
