#include "Logger.h"
#include <cstdarg>
#include <vector>
#include <pthread.h>
#include <boost/format.hpp>


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
    unsigned int m_lineNumberCount;
    unsigned int m_pad;
    Colours m_colour;
    std::string m_logfileName;
    TeeStream m_output;
    std::ofstream m_file;
    pthread_mutex_t m_mutex;
    std::string m_timeString;

    Impl(const std::string &_fname);
    void write(const std::string &_text);
    void writeLineNumber();
    void writeTimeStamp();
    std::string currentTime();
    void setColour(enum Colours c);


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
                            m_colour(RESET),
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
    m_timeString="%I:%M%p";
  }
  std::string NCCALogger::Impl::currentTime()
  {
  std::string timeStr;
  time_t rawTime;
  struct tm * timeinfo;
  char buffer [80];
  time( & rawTime );
  timeStr = ctime( &rawTime );
  timeinfo = localtime (&rawTime);
  //without the newline character
   strftime (buffer,80,m_timeString.c_str(),timeinfo);
  return buffer;
  }


  void NCCALogger::Impl::writeLineNumber()
  {
    setColour(m_colour);
    if(m_lineNumber == true)
    {
      pthread_mutex_lock (&m_mutex);

      std::string st=boost::str(boost::format("%%0%dd") %m_pad );
      std::string t=boost::str(boost::format(st.c_str()) %++m_lineNumberCount);
      m_output<<t<<" ";
      pthread_mutex_unlock (&m_mutex);
    }
  }
  void NCCALogger::Impl::writeTimeStamp()
  {
    pthread_mutex_lock (&m_mutex);

    setColour(m_colour);

    if(m_timeStamp==true)
    {
      m_output<<currentTime()<<" ";
    }
    pthread_mutex_unlock (&m_mutex);

  }

  void NCCALogger::Impl::write(const std::string &_text)
  {
    pthread_mutex_lock (&m_mutex);

    setColour(m_colour);
    m_output<<_text;
    pthread_mutex_unlock (&m_mutex);

  }

  // from http://stackoverflow.com/questions/3585846/color-text-in-terminal-aplications-in-unix
  void NCCALogger::Impl::setColour(enum Colours c)
  {
    if(m_disableColours) return;
    pthread_mutex_lock (&m_mutex);

    switch(c)
    {
      case NORMAL : m_output<<"\x1B[0m"; break;
      case RED : m_output<<"\x1B[31m"; break;
      case GREEN : m_output<<"\x1B[32m"; break;
      case YELLOW : m_output<<"\x1B[33m"; break;
      case BLUE : m_output<<"\x1B[34m"; break;
      case MAGENTA : m_output<<"\x1B[35m"; break;
      case CYAN : m_output<<"\x1B[36m"; break;
      case WHITE : m_output<<"\x1B[37m"; break;
      case RESET : m_output<<"\033[0m"; break;
      default : m_output<<"\033[0m"; break;
    }

    pthread_mutex_unlock (&m_mutex);

  }



  NCCALogger::NCCALogger() : m_impl(new NCCALogger::Impl("output.log"))
  {
    m_impl->setColour(BLUE);
    m_impl->m_output<<"NCCALogger started "<<m_impl->currentTime()<<"\n";
    m_impl->setColour(RESET);
  }

  NCCALogger::NCCALogger(const std::string &_fname) : m_impl(new NCCALogger::Impl(_fname))
  {
    pthread_mutex_lock (&m_impl->m_mutex);

    m_impl->setColour(BLUE);
    m_impl->m_output<<"NCCALogger started "<<m_impl->currentTime()<<"\n";
    m_impl->setColour(RESET);
    pthread_mutex_unlock (&m_impl->m_mutex);

  }

  NCCALogger::~NCCALogger()
  {
    m_impl->setColour(RESET);
    m_impl->m_output<<"\n";
    m_impl->m_output.flush();
    m_impl->m_output.close();
  }




  void NCCALogger::logMessage(const char *fmt,...)
  {
    pthread_mutex_lock (&m_impl->m_mutex);
    m_impl->writeLineNumber();
    m_impl->writeTimeStamp();
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    std::string text=buffer;
    va_end (args);
    m_impl->write(text);
    fflush(stdout);
    pthread_mutex_unlock(&m_impl->m_mutex);
  }

  void NCCALogger::logError(const char* fmt,...)
  {
    pthread_mutex_lock (&m_impl->m_mutex);
    m_impl->writeLineNumber();
    m_impl->writeTimeStamp();
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    m_impl->setColour(RED);
    m_impl->m_output<<"[ERROR] ";
    std::string text=buffer;
    va_end (args);
    m_impl->write(text);
    fflush(stdout);
    pthread_mutex_unlock(&m_impl->m_mutex);

  }

  void NCCALogger::logWarning(const char* fmt...)
  {
    pthread_mutex_lock (&m_impl->m_mutex);
    m_impl->writeLineNumber();
    m_impl->writeTimeStamp();
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    m_impl->setColour(GREEN);
    m_impl->m_output<<"[Warning] ";
    std::string text=buffer;
    va_end (args);
    m_impl->write(text);
    fflush(stdout);
    pthread_mutex_unlock(&m_impl->m_mutex);

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
    m_impl->m_output.flush();
    m_impl->m_output.close();

    m_impl->m_file.close();
    m_impl->m_logfileName=_fname;
    m_impl->m_file.open(m_impl->m_logfileName.c_str() );
    if(!m_impl->m_file.is_open())
    {
     std::cerr<<"error opening log file for writing\n";
     exit(EXIT_FAILURE);
    }
     Tee tee( std::cout, m_impl->m_file );

     m_impl->m_output.open(tee);
     m_impl->m_output.set_auto_close(true);

    if( !m_impl->m_output.is_open())
    {
      std::cerr<<"problem opening log stream tee\n";
      exit(EXIT_FAILURE);
    }




  }
  void NCCALogger::setColour(Colours _c)
  {
    pthread_mutex_lock (&m_impl->m_mutex);

    m_impl->m_colour=_c;
    pthread_mutex_unlock (&m_impl->m_mutex);
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

  boost::iostreams::stream<NCCALogger::Tee> &NCCALogger::cout()
  {

    return m_impl->m_output;
  }

//Fri Nov 21 12:20:09 2014
  void NCCALogger::setTimeFormat(TimeFormat _f)
  {
    switch(_f)
    {
      case TIME : m_impl->m_timeString="%I:%M%p"; break;
      case TIMEDATE : m_impl->m_timeString="%R %D"; break;
      case TIMEDATEDAY :m_impl->m_timeString="%c"; break;
    }
  }
}
