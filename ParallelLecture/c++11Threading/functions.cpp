#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <functional>

#include "Logger.h"

void foo(const std::string  &a, const std::string &b)
{
	while(1)
	nccalog::NCCALogger::instance().
	logMessage("foo(str,str) ID %d value %s %s \n"
		,std::this_thread::get_id(),a.c_str(),b.c_str());
}

void foo(int a)
{
	while(1)
	nccalog::NCCALogger::instance().logMessage("foo(int) ID %d value %d \n"
		,std::this_thread::get_id(),a);
}

void foo(double a)
{
	while(1)
	nccalog::NCCALogger::instance().logMessage("foo(double) ID %d value %f\n"
		,std::this_thread::get_id(),a);
}



int main()
{
  std::vector<std::thread> threads;
  threads.reserve(4);
  nccalog::NCCALogger::instance().setColour(nccalog::Colours::CYAN);
  nccalog::NCCALogger::instance().logWarning("creating thread String Function\n");

	
	auto funca = std::bind<void(int)>(foo,1);
	threads.emplace_back(funca);

	auto funcb = std::bind<void(double)>(foo,0.002);
	threads.emplace_back(funcb);

  std::string a="hello";
  std::string b=" c++ 11 threads";
	auto funcs = std::bind<void(const std::string &,const std::string &)>(foo,a,b);
	threads.emplace_back(funcs);

	using namespace std::placeholders;  // for _1, _2, _3...
	auto funcs2 = std::bind<void(const std::string &,const std::string &)>(foo,_1,_2);
	threads.emplace_back(funcs2,"placeholders","are cool");


  int i=0;
  for(auto& thread : threads)
  {
      nccalog::NCCALogger::instance().setColour(nccalog::Colours::YELLOW);
      nccalog::NCCALogger::instance().logWarning("Joining thread %d\n",i++);
      thread.join();
  }

  return EXIT_SUCCESS;
}