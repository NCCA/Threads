  template<typename T, typename... args>

  void NCCALogger::Impl::logMessage(const char* format, T value, args... arg)
  {
    for ( ; *format != '\0'; format++ )
    {
      if ( *format == '%' )
      {
        write(value);
        LogMessage(format+1, arg...); // recursive call
        return;
      }
    write(format);
    }
  }
