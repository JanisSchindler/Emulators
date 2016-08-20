#include "logger.h"

Logger* sInstance = NULL;
std::ofstream* mLogFile = NULL;

Logger* Logger::getInstance(const QString logFileName)
{
  if (NULL == sInstance)
  {
    if (NULL != logFileName)
    {
      sInstance = new Logger(logFileName.toStdString());
    }
    else
    {
      qDebug() << "Set File name prior to calling getInstance";
    }
  }
  return sInstance;
}

void Logger::cleanup()
{
  if (NULL == sInstance)
  {
    return;
  }
  sInstance = NULL;
  if (NULL != mLogFile)
  {
    if (mLogFile->is_open())
    {
      mLogFile->close();
    }
    delete mLogFile;
    mLogFile = NULL;
  }
}

Logger::Logger(const std::string logFileName)
{
  try
  {
    mLogFile = new std::ofstream();
    mLogFile->open(logFileName.c_str());
  }
  catch(...)
  {
    qDebug() << "Could not open file for logger";
  }
}

void Logger::log(const QString message)
{
  try
  {
    qDebug() << message;
    if (!mLogFile->is_open())
    {
      return;
    }
    QString logEntry = QDateTime::currentDateTime().toString("dd.MM.yyyy/hh:mm:ss");
    logEntry.append('\t');
    logEntry.append(message);
    *mLogFile << logEntry.toStdString() << "\n";
  }
  catch(...)
  {
    qDebug() << "Error logging to file";
  }
}

int mFlags;

void Logger::logOnce(const QString message, const int flag)
{
  if (mFlags & flag)
  {
    return;
  }
  mFlags |= flag;
  log(message);
}
