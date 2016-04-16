#include "mainwindow.h"
#include "ui_mainwindow.h"

DWORD mAppHandleId = 0;

MainWindow::MainWindow(char *argv[], QWidget *parent) :
  QMainWindow(parent),
  mUi(new Ui::MainWindow)
{
  // init logging
  QString logFile = QString(argv[0]);
  logFile.append(".log");
  Logger::getInstance(logFile)->log("Starting...");

  mModel = new ViewModel();

  mUi->setupUi(this);
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
  connect(ControllerInput::getInstance(), SIGNAL(keyPressed(Input::Keys)), this, SLOT(onControllerInput(Input::Keys)));
  connect(mUi->mListEmulators, SIGNAL(currentRowChanged(int)), this, SLOT(onCurrentRowChanged(int)));
  connect(mUi->mListRoms, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(onRomListItemDoubleClicked(QListWidgetItem*)));

  // init focus
  mUi->mListEmulators->setFocus();
  mUi->mListEmulators->setCurrentRow(0);
  mUi->mListRoms->setCurrentRow(0);
}

MainWindow::~MainWindow()
{
  disconnect(ControllerInput::getInstance(), SIGNAL(keyPressed(Input::Keys)), this, SLOT(onControllerInput(Input::Keys)));
  disconnect(mUi->mListEmulators, SIGNAL(currentRowChanged(int)), this, SLOT(onCurrentRowChanged(int)));
  disconnect(mUi->mListRoms, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onRomListItemDoubleClicked(QListWidgetItem*)));
  delete mUi;
  ControllerInput::cleanup();
  Logger::cleanup();
  delete mModel;
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
  if (e->key() ==Qt::Key_Escape)
  {
    onControllerInput(Input::Back);
  }
  else if (e->key() == Qt::Key_Return)
  {
    onControllerInput(Input::Accept);
  }
  // pass to Qt class
  QMainWindow::keyPressEvent(e);
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
  try
  {
    LPCWSTR applicationName = emulator->executablePath;

    QString commandLine = QString::fromStdWString(emulator->executablePath);
    commandLine.append(" ");
    commandLine.append(QString::fromStdWString(emulator->arguments));

    // build command line -> replace the <rom> macro and put it in hyphens
    // add a space - some programs seem to require it
    QString romPath(" \"");
    romPath.append(QString::fromStdWString(rom->path));
    romPath.append("\"");
    commandLine.replace("<ROM>", romPath, Qt::CaseInsensitive);
    Logger::getInstance()->log("CommandLine: " + commandLine);


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

    // Close process and thread handles, remember id
    mAppHandleId = (pi.dwProcessId);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
  catch(...)
  {
    Logger::getInstance()->log("Error starting emulator");
  }
}

void MainWindow::onRomListItemDoubleClicked(QListWidgetItem* item)
{
  // get the emulator
  const Emulator* emulator = mModel->getEmulatorForIndex(mUi->mListEmulators->currentRow());
  if (!emulator->valid)
  {
    return;
  }
  // get the rom
  const ROM* rom = mModel->getRomForIndex(emulator, mUi->mListRoms->currentRow());
  if (!rom->valid)
  {
    return;
  }
  StartROM(emulator, rom);
}

// So what does this do?
// This is passed as callback for EnumWindows (below)
// in it we compare the Window's process's handle
// to the one we remembered upon creation of the emulator
// and then tell it to close
// not sure if this is the kosher way but it works...
WINBOOL CALLBACK findAndClose (HWND hwnd, LPARAM lParam)
{
  DWORD procid;
  GetWindowThreadProcessId (hwnd, &procid);
  if (0 != mAppHandleId && procid == mAppHandleId)
  {
    // close the emulator
    PostMessage(hwnd, WM_CLOSE, 0, 0);
    return FALSE; // found and stopped
  }
  return TRUE; // continue iterating
}

WINBOOL CALLBACK findAndEscape (HWND hwnd, LPARAM lParam)
{
  DWORD procid;
  GetWindowThreadProcessId (hwnd, &procid);
  if (0 != mAppHandleId && procid == mAppHandleId)
  {
    // close the emulator
    PostMessage(hwnd, WM_KEYDOWN,  VK_ESCAPE, 0);
    return FALSE; // found and stopped
  }
  return TRUE; // continue iterating
}

// second pass, if found here it seemed not to have reacted to WM_CLOSE
WINBOOL CALLBACK findAndKill (HWND hwnd, LPARAM lParam)
{
  DWORD procid;
  GetWindowThreadProcessId (hwnd, &procid);
  if (0 != mAppHandleId && procid == mAppHandleId)
  {
    Logger::getInstance()->log("Process seemed not to react to WM_CLOSE");
    // close the emulator
    TerminateProcess(hwnd, 0);
    return FALSE; // found and stopped
  }
  return TRUE; // continue iterating
}

void MainWindow::onControllerInput(Input::Keys keys)
{
  if (keys == Input::None)
  {
    return;
  }
  if (keys & Input::Exit && 0 != mAppHandleId)
  {
    try
      {
      // try the nice way (send WM_CLOSE)
      EnumWindows(findAndClose, 0);
      Sleep(1000);
      // dolphin workaround -> send esc
      EnumWindows(findAndEscape, 0);
      Sleep(1000);
      // try the not so nice way (TerminateProcess)
      EnumWindows(findAndKill, 0);
      // reset handle
      mAppHandleId = 0;

      return;
      }
    catch(...)
    {
      Logger::getInstance()->log("Error closing emulator");
    }
  }
  // exit this progrma
  if (keys & Input::Exit && keys & Input::Accept && 0 == mAppHandleId &&
      (mUi->mListEmulators->hasFocus() || mUi->mListRoms->hasFocus()))
  {
    exit(0);
  }
  // note: the following hasFocus() checks also prevent us
  // triggering these if an emulator is running (and thus has focus)
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
