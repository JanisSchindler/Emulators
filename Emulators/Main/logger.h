#ifndef LOGGER_H
#define LOGGER_H

#include <QtDebug>
#include <QDateTime>
#include <iostream>
#include <fstream>

class Logger
{
public:
  static Logger* getInstance();
  static void cleanup();
  void setLogFile(const QString logFile);
  void log(const QString message);
  void logOnce(const QString message, int flag);

private:
  char* mLogFilePath;
  int mFlags;
  static Logger* sInstance;
  Logger();
};

#endif // LOGGER_H
