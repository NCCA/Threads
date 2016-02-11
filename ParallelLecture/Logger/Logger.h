#ifndef LOGGER_H__
#define LOGGER_H__
#include <boost/scoped_ptr.hpp>
#include <cstdarg>
#include <iostream>
#include <ostream>
#include <fstream>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/tee.hpp>

/// @brief logger class for writing to stream and file.
namespace nccalog
{
  enum Colours{NORMAL,RED,GREEN ,YELLOW,BLUE,MAGENTA,CYAN,WHITE,RESET};
  enum TimeFormat{TIME,TIMEDATE,TIMEDATEDAY};
  class NCCALogger
  {
    public :

      NCCALogger();
      NCCALogger(const std::string &_fname);
      ~NCCALogger();
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
      typedef boost::iostreams::tee_device<std::ostream, std::ofstream > Tee;
      typedef boost::iostreams::stream<Tee> TeeStream;
      boost::iostreams::stream<NCCALogger::Tee> &cout();

    private :
      class Impl;
      boost::scoped_ptr<Impl> m_impl;


  };

} // end namespace

#endif
