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

void DoLoadRomsForEmulator(QString path, const Emulator* emulator, ViewModel* model, QString extension)
{
  QDir dir(path);
  QFileInfoList infos = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  for (int i = 0; i < infos.length(); ++i)
  {
    if (infos[i].isDir())
    {
      DoLoadRomsForEmulator(infos[i].absolutePath(), emulator, model, extension);
    }
    if(infos[i].completeSuffix() == extension)
    {
      ROM* rom = new ROM();
      rom->name = infos[i].baseName().toStdString();
      rom->path = infos[i].absoluteFilePath().toStdWString();
      model->AddRom(emulator, rom);
    }
  }
}

// loads the settings from the emulator ini file and
// adds the ROMS to the model
void LoadRomsForEmulator(Emulator* emulator, ViewModel* model)
{
  // search for another ini file inside the emulator folder
  QFileInfo file(QString::fromWCharArray(emulator->executablePath));
  QString path = file.absolutePath();
  path = QDir::toNativeSeparators(path);
  if (path[path.length() - 1] != '\\')
  {
    path.append('\\');
  }

  wchar_t* pattern;
  ConcatenateStrings(path.toStdWString().c_str(), L"*.ini", pattern);

  HANDLE handle;
  WIN32_FIND_DATA findIniFile;

  qDebug() << "ini file pattern " << QString::fromWCharArray(pattern, wcslen(pattern));

  // Todo: consider the case there are multiple ini files!
  handle = FindFirstFile(pattern, &findIniFile);
  if (handle == INVALID_HANDLE_VALUE || handle == 0)
  {
    // we need the ini file for the file ending at least
    return;
  }

  wchar_t* iniFile;
  ConcatenateStrings(path.toStdWString().c_str(), findIniFile.cFileName, iniFile);
  FindClose(handle);

  // buffer
  wchar_t* iniEntry = new wchar_t[MAX_PATH - 1];
  // read extension of ROMs for this emulator
  DWORD res = GetPrivateProfileString
      (
        L"EMULATOR",     // section
        L"ROMExtension", // key
        L"",             // default
        iniEntry,        // target buffer
        MAX_PATH - 1,    // buffer size
        iniFile          // ini file
      );
  QString extension = QString::fromWCharArray(iniEntry, res);

  // read folder for ROMS of this emulator
  res = GetPrivateProfileString
      (
        L"EMULATOR",     // section
        L"ROMFolder",    // key
        L"",             // default
        iniEntry,        // target buffer
        MAX_PATH - 1,    // buffer size
        iniFile          // ini file
      );

  QString romFolder = QString::fromWCharArray(iniEntry, res);
  path.append(romFolder);
  if (path[path.length() - 1] != '\\')
  {
    path.append('\\');
  }
  qDebug() << "root folder for rom lookup: " << path;

  // read Command Line arguments for this emulator
  res = GetPrivateProfileString
      (
        L"EMULATOR",        // section
        L"CommandLineArgs",  // key
        L"",                // default
        iniEntry,            // target buffer
        MAX_PATH - 1,      // buffer size
        iniFile            // ini file
      );
  // reuse buffer in model
  emulator->arguments = iniEntry;

  // Todo: read DisplayName from Ini file

  DoLoadRomsForEmulator(path, emulator, model, extension);
}

// search recursively in root for .exefiles for emulators
// those then should contain the path and extension for their roms
// as well as the command line parameters that are required
void LoadEmulatorsInDirectory(QDir dir, ViewModel* model)
{
  QFileInfoList infos = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  for(int i = 0; i < infos.length(); ++i)
  {
    if(infos[i].isDir())
    {
      LoadEmulatorsInDirectory(infos[i].absoluteFilePath(), model);
    }
    if (!infos[i].isFile())
    {
      continue;
    }
    if (infos[i].completeSuffix() == "exe")
    {
      Emulator* e = new Emulator();

      int length = infos[i].absoluteFilePath().length() + 1;
      wchar_t* p = new wchar_t[length];

      infos[i].absoluteFilePath().toWCharArray(p);
      // add null terminator
      p[length - 1] = '\0';
      e->executablePath = p;
      e->name = infos[i].baseName().toStdString();
      model->AddEmulator(e);
      LoadRomsForEmulator(e, model);
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
    dir = QDir(QString::fromWCharArray(rootDir));
    delete rootDir;
    LoadEmulatorsInDirectory(dir, model);
    return;
  }
  // else use the path in the ini file
  // thus first get the path OF the ini file
  // note that findIniFile contains a file NAME

  wchar_t* iniFile;
  ConcatenateStrings(pathRoot, findIniFile.cFileName, iniFile);
  FindClose(handle);

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
  if (res >= MAX_PATH)
  {
    // try program folder as root folder
    rootDir = new wchar_t[res + 1];
    CopyString(exePath, rootDir, res + 1, true);
  }

  delete pattern;
  delete iniFile;
  dir = QDir(QString::fromWCharArray(rootDir));
  delete rootDir;
  LoadEmulatorsInDirectory(dir, model);
}
