#include "loader.h"

void CopyString(const char* from, wchar_t* to, uint count, bool terminate)
{
  // is there a convenient way to copy char* to wchar_t* ?
  for (uint i = 0; i < count; ++i)
  {
    to[i] = from[i];
    if (from[i] == '\0')
    {
      // stop at terminated string
      return;
    }
    if (i == count - 1 && terminate)
    {
      to[i] = '\0';
    }
  }
}

// creates a string -> delete after use!
void ConcatenateStrings(const wchar_t* part1, const wchar_t* part2, wchar_t*& result)
{
  int len = wcslen(part1) + wcslen(part2) + 1;
  result = new wchar_t[len];
  wcscpy_s(result, len, part1);
  wcscat_s(result, len, part2);
}

// search recursively in root for .exe and .ini files for emulators
// those then should contain the path and extension for their roms
// as well as the command line parameters that are required
void LocateInDirectory(QDir dir, ViewModel* model)
{
  QFileInfoList infos = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  for(int i = 0; i < infos.length(); ++i)
  {
    if(infos[i].isDir())
    {
      LocateInDirectory(infos[i].absoluteFilePath(), model);
    }
    if (!infos[i].isFile())
    {
      continue;
    }
    if (infos[i].completeSuffix() == "exe")
    {
      Emulator* e = new Emulator();
      e->executablePath = infos[i].absoluteFilePath().toStdWString().c_str();
      e->name = infos[i].baseName().toStdString();
      model->AddEmulator(e);
    }
    if (infos[i].completeSuffix() == "zip")
    {
      int current = model->getEmulatorCount();
      if (current <= 0)
      {
        continue;
      }
      ROM* r = new ROM();
      const Emulator* last = model->getEmulatorForIndex(current - 1);
      r->path =  infos[i].absoluteFilePath().toStdWString().c_str();
      r->name = infos[i].baseName().toStdString();
      model->AddRom(last, r);
    }
  }
}

void Loader::Load(char exePath[], ViewModel* model)
{
  wchar_t* rootDir;
  QDir dir;
  // find ini file in executable directory
  // this should contain a path to the root directory where to
  // look for emulators
  HANDLE handle;
  WIN32_FIND_DATA findIniFile;

  int pathLen = strlen(exePath);

  for (int i = pathLen; i > 0; --i)
  {
    if (exePath[i] == '\\')
    {
      // only use up to last directory separator
      pathLen = i + 1;
      break;
    }
  }

  wchar_t pathRoot[pathLen + 1];
  CopyString(exePath, pathRoot, pathLen + 1, true);

  wchar_t* pattern;
  ConcatenateStrings(pathRoot, L"*.ini", pattern);

  qDebug() << "pattern: " << QString::fromWCharArray(pattern, wcslen(pattern));


  handle = FindFirstFile(pattern, &findIniFile);
  if (handle == INVALID_HANDLE_VALUE || handle == 0)
  {
    // try program folder as root folder
    rootDir = new wchar_t[pathLen + 1];
    CopyString(exePath, rootDir, pathLen + 1, true);
    delete pattern;
    dir = QDir(QString::fromWCharArray(rootDir, wcslen(rootDir)));
    delete rootDir;
    LocateInDirectory(dir, model);
    return;
  }
  // else use the path in the ini file
  // thus first get the path OF the ini file
  // note that findIniFile contains a file NAME

  wchar_t* iniFile;
  ConcatenateStrings(pathRoot, findIniFile.cFileName, iniFile);

  rootDir = new wchar_t[MAX_PATH - 1];
  qDebug() << "ini: " << QString::fromWCharArray(iniFile, wcslen(iniFile));
  DWORD res = GetPrivateProfileString
      (
        L"Emulators",    // section
        L"RootDirectory",// key
        L"",             // default
        rootDir,         // target buffer
        MAX_PATH - 1,    // buffer size
        iniFile          // ini file
      );
  if (res < 0 || res >= MAX_PATH)
  {
    // try program folder as root folder
    rootDir = new wchar_t[res + 1];
    CopyString(exePath, rootDir, res + 1, true);
  }

  delete pattern;
  delete iniFile;
  dir = QDir(QString::fromWCharArray(rootDir, wcslen(rootDir)));
  delete rootDir;
  LocateInDirectory(dir, model);
}
