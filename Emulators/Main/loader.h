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
  static void Load(char* exePath, ViewModel* model);
};

#endif // LOADER_H
