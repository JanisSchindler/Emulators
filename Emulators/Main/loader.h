#ifndef LOADER_H
#define LOADER_H

#include <windows.h>
#include <fstream>
#include <QDir>
#include <QDebug>
#include "viewmodel.h"

class Loader
{
public:
  static void Load(char* exePath, ViewModel* model);
};

#endif // LOADER_H
