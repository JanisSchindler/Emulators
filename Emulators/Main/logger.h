#ifndef LOGGER_H
#define LOGGER_H

#include <QtDebug>
#include <QDateTime>
#include <iostream>
#include <fstream>

class Logger
{
public:  
  static Logger* getInstance(const QString logFileName = NULL);
  static void cleanup();
  void log(const QString message);
  void logOnce(const QString message, int flag);

private:
  Logger(const std::string logFileName);
};

#endif // LOGGER_H
