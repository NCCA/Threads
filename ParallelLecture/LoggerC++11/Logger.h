#ifndef LOGGER_H_
#define LOGGER_H_
#include <memory>
#include <cstdarg>
#include <iostream>
#include <ostream>
#include <fstream>

/// @brief logger class for writing to stream and file.
namespace nccalog
{
  enum class Colours{NORMAL,RED,GREEN ,YELLOW,BLUE,MAGENTA,CYAN,WHITE,RESET};
  enum class TimeFormat{TIME,TIMEDATE,TIMEDATEDAY};
  class NCCALogger
  {
    private:
      NCCALogger();
      ~NCCALogger();
      NCCALogger(const NCCALogger &)=delete;
      NCCALogger & operator=(const NCCALogger &)=delete;
    public :
      static NCCALogger &instance();
      void logMessage(const char* fmt, ...);
      void logError(const char* fmt, ...);
      void logWarning(const char* fmt, ...);
      void enableLogToFile();
      void disableLogToFile();
      void enableLogToConsole();
      void disableLogToConsole();
      void enableLogToFileAndConsole();
      void disableLogToFileAndConsole();
      void setLogFile(const std::string &_fname);
      void setColour(Colours _c);
      void enableLineNumbers();
      void disableLineNumbers();
      void enableTimeStamp();
      void disableTimeStamp();
      void disableColours();
      void enableColours();
      void setLineNumberPad(unsigned int i);
      void setTimeFormat(TimeFormat _f);

    private :
      class Impl;
      std::unique_ptr<Impl> m_impl;


  };

} // end namespace

#endif
