#include "logger.h"

Logger* Logger::sInstance = NULL;

Logger::Logger(){}

Logger* Logger::getInstance()
{
  if (NULL == sInstance)
  {   
    sInstance = new Logger();
  }
  return sInstance;
}

void Logger::cleanup()
{
  if (NULL == sInstance)
  {
    return;
  }
  delete (sInstance->mLogFilePath);
  sInstance = NULL;
}

void Logger::setLogFile(const QString logFile)
{
  mLogFilePath = new char[logFile.length() + 1];
  wchar_t buffer[logFile.length()];
  logFile.toWCharArray(buffer);
  for (int i = 0; i < logFile.length(); ++i)
  {
    // make char* from wchar_t*
    mLogFilePath[i] = buffer[i];
  }
  mLogFilePath[logFile.length()] = '\0';
  // remove old log file (maybe change this sometime?)
  std::ofstream ofs;
  ofs.open(mLogFilePath, std::ofstream::out | std::ofstream::trunc);
  ofs.close();
}

void Logger::log(const QString message)
{
  try
  {
    qDebug() << message;
    std::ofstream file;
    file.open(mLogFilePath, std::ofstream::out | std::ofstream::app);
    if (!file.is_open())
    {
        qDebug() << "Error logging to file";
        return;
    }
    QString logEntry = QDateTime::currentDateTime().toString("dd.MM.yyyy/hh:mm:ss");
    logEntry.append('\t');
    logEntry.append(message);
    file << logEntry.toStdString() << "\n";
    file.close();
  }
  catch(...)
  {
    qDebug() << "Error logging to file";
  }
}

void Logger::logOnce(const QString message, const int flag)
{
  if (mFlags & flag)
  {
    return;
  }
  mFlags |= flag;
  log(message);
}
