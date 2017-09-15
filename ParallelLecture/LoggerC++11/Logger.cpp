#include "Logger.h"
#include <cstdarg>
#include <vector>
#include <boost/format.hpp>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>
namespace nccalog
{
  // PIMPL Idiom to make lib cleaner
  class NCCALogger::Impl
  {

  public:
    bool m_logFileAndConsole;
    bool m_logFile;
    bool m_logConsole;
    bool m_timeStamp;
    bool m_lineNumber;
    bool m_disableColours;
    std::atomic<unsigned int> m_lineNumberCount;
    unsigned int m_pad;
    std::atomic<Colours> m_colour;
    std::string m_colourString;
    std::string m_logfileName;
    typedef boost::iostreams::tee_device<std::ostream, std::ofstream > Tee;
    typedef boost::iostreams::stream<Tee> TeeStream;

    std::ofstream m_file;
    std::mutex m_writeStreamMutex;
    std::mutex m_setColourMutex;
    std::mutex m_writeNumberMutex;
    std::mutex m_writeTimeMutex;
    std::condition_variable m_waitColour;
    bool ready = false;
    std::string m_timeString;

    Impl(const std::string &_fname);
    void write(const std::string &_text);
    void writeLineNumber();
    void writeTimeStamp();
    void currentTime();
    void setColour(enum Colours c);
    void flush(){m_output.flush();}
    void close(){m_output.close();}
    void reopen(Tee tee)
    {
      m_output.open(tee);
      m_output.set_auto_close(true);
      if( !m_output.is_open())
      {
        std::cerr<<"problem opening log stream tee\n";
        exit(EXIT_FAILURE);
      }


    }

  private :
    TeeStream m_output;

  };





  NCCALogger::Impl::Impl(const std::string &_fname):
                            m_logFileAndConsole(false),
                            m_logFile(true),
                            m_logConsole(false),
                            m_timeStamp(true),
                            m_lineNumber(true),
                            m_disableColours(false),
                            m_lineNumberCount(0),
                            m_pad(4),
                            m_colour(Colours::RESET),
                            m_logfileName(_fname)
  {

    m_file.open(m_logfileName.c_str() );
    if(!m_file.is_open())
    {
     std::cerr<<"error opening log file for writing\n";
     exit(EXIT_FAILURE);
    }
     Tee tee( std::cout, m_file );

     m_output.open(tee);
     m_output.set_auto_close(true);

    if( !m_output.is_open())
    {
      std::cerr<<"problem opening log stream tee\n";
      exit(EXIT_FAILURE);
    }
    m_timeString="%I:%M:%S%p";
    m_colourString="\x1B[0m";
  }

  void NCCALogger::Impl::currentTime()
  {
    std::lock_guard<std::mutex> lock(m_writeTimeMutex);

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream time;
    time <<std::put_time(std::localtime(&now_c), m_timeString.c_str());
    time<<" ";
    write(time.str());
  }


  void NCCALogger::Impl::writeLineNumber()
  {
    std::lock_guard<std::mutex> lock(m_writeNumberMutex);

    setColour(m_colour);
    if(m_lineNumber == true)
    {
      std::string st=boost::str(boost::format("%%0%dd") %m_pad );
      std::string t=boost::str(boost::format(st.c_str()) %++m_lineNumberCount);
      write(t);
      write(" ");
    }
  }
  void NCCALogger::Impl::writeTimeStamp()
  {

    setColour(m_colour);

    if(m_timeStamp==true)
    {
      currentTime();
    }

  }

  void NCCALogger::Impl::write(const std::string &_text)
  {

    std::lock_guard<std::mutex> lock(m_writeStreamMutex);
    m_output<<m_colourString;
    m_output<<_text;

  }
  // from http://stackoverflow.com/questions/3585846/color-text-in-terminal-aplications-in-unix
  void NCCALogger::Impl::setColour(enum Colours c)
  {
    if(m_disableColours) return;

    switch(c)
    {
      case Colours::NORMAL : m_colourString="\x1B[0m"; break;
      case Colours::RED : m_colourString="\x1B[31m"; break;
      case Colours::GREEN : m_colourString="\x1B[32m"; break;
      case Colours::YELLOW : m_colourString="\x1B[33m"; break;
      case Colours::BLUE : m_colourString="\x1B[34m"; break;
      case Colours::MAGENTA : m_colourString="\x1B[35m"; break;
      case Colours::CYAN : m_colourString="\x1B[36m"; break;
      case Colours::WHITE : m_colourString="\x1B[37m"; break;
      case Colours::RESET : m_colourString="\033[0m"; break;
      default : m_colourString="\033[0m"; break;
    }


  }

  NCCALogger & NCCALogger::instance()
  {
    static NCCALogger instance;
    return instance;
  }


  NCCALogger::NCCALogger() : m_impl(new NCCALogger::Impl("output.log"))
  {
    m_impl->setColour(Colours::BLUE);
    m_impl->write("NCCALogger started ");
    m_impl->currentTime();
    m_impl->write("\n");
    m_impl->setColour(Colours::RESET);
  }



  NCCALogger::~NCCALogger()
  {
    m_impl->setColour(Colours::RESET);
    m_impl->write("\n");
    m_impl->flush();
    m_impl->close();
  }




  void NCCALogger::logMessage(const char *fmt,...)
  {

    m_impl->writeLineNumber();
    m_impl->writeTimeStamp();
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    std::string text=buffer;
    va_end (args);

    m_impl->write(text);
  }

  void NCCALogger::logError(const char* fmt,...)
  {

    m_impl->writeLineNumber();
    m_impl->writeTimeStamp();
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    m_impl->setColour(Colours::RED);
    m_impl->write("[ERROR] ");
    setColour(m_impl->m_colour);
    std::string text=buffer;
    va_end (args);
    m_impl->write(text);

  }

  void NCCALogger::logWarning(const char* fmt...)
  {

    m_impl->writeLineNumber();
    m_impl->writeTimeStamp();
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    m_impl->setColour(Colours::GREEN);
    m_impl->write("[Warning] ");
    setColour(m_impl->m_colour);

    std::string text=buffer;
    va_end (args);
    m_impl->write(text);

  }


  void NCCALogger::enableLogToFile()
  {
    m_impl->m_logFile=true;

  }
  void NCCALogger::disableLogToFile()
  {
    m_impl->m_logFile=false;

  }
  void NCCALogger::enableLogToConsole()
  {
    m_impl->m_logConsole=true;

  }
  void NCCALogger::disableLogToConsole()
  {
    m_impl->m_logConsole=false;

  }
  void NCCALogger::enableLogToFileAndConsole()
  {
    m_impl->m_logConsole=true;
    m_impl->m_logFile=true;
  }
  void NCCALogger::disableLogToFileAndConsole()
  {
    m_impl->m_logConsole=false;
    m_impl->m_logFile=false;

  }
  void NCCALogger::setLogFile(const std::string &_fname)
  {
    // close the file
    m_impl->flush();
    m_impl->close();

    m_impl->m_file.close();
    m_impl->m_logfileName=_fname;
    m_impl->m_file.open(m_impl->m_logfileName.c_str() );
    if(!m_impl->m_file.is_open())
    {
     std::cerr<<"error opening log file for writing\n";
     exit(EXIT_FAILURE);
    }
     Impl::Tee tee( std::cout, m_impl->m_file );

     m_impl->reopen(tee);




  }
  void NCCALogger::setColour(Colours _c)
  {
    std::lock_guard<std::mutex> lock(m_impl->m_setColourMutex);
    m_impl->m_colour=_c;
  }
  void NCCALogger::enableLineNumbers()
  {
    m_impl->m_lineNumber=true;
  }
  void NCCALogger::disableLineNumbers()
  {
    m_impl->m_lineNumber=false;

  }
  void NCCALogger::enableTimeStamp()
  {
    m_impl->m_timeStamp=true;
  }
  void NCCALogger::disableTimeStamp()
  {
    m_impl->m_timeStamp=false;
  }

  void NCCALogger::disableColours()
  {
    m_impl->m_disableColours=true;
  }

  void NCCALogger::enableColours()
  {
    m_impl->m_disableColours=false;
  }

  void NCCALogger::setLineNumberPad(unsigned int _i)
  {
    m_impl->m_pad=_i;
  }


//Fri Nov 21 12:20:09 2014
  void NCCALogger::setTimeFormat(TimeFormat _f)
  {
    switch(_f)
    {
      case TimeFormat::TIME : m_impl->m_timeString="%I:%M:%S%p"; break;
      case TimeFormat::TIMEDATE : m_impl->m_timeString="%R %D"; break;
      case TimeFormat::TIMEDATEDAY :m_impl->m_timeString="%c"; break;
    }
  }
}
