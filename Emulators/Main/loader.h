#ifndef LOADER_H
#define LOADER_H

#include <windows.h>
#include <fstream>
#include <QDir>
#include "viewmodel.h"
#include "logger.h"

class Loader
{
public:
  static void load(char* exePath, ViewModel* model);

private:
  static void copyString(const char* from, wchar_t* to, uint count, bool terminate);
  static void concatenateStrings(const wchar_t* part1, const wchar_t* part2, wchar_t*& result);
  static bool checkIniFile(QString path);
  static void doLoadRomsForEmulator(QString path, const Emulator* emulator, ViewModel* model, QString extension);
  static void loadRomsForEmulator(Emulator* emulator, ViewModel* model);
  static void loadEmulatorsInDirectory(QDir dir, ViewModel* model);
};

#endif // LOADER_H
