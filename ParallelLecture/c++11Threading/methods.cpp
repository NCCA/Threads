#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <functional>

#include "Logger.h"

nccalog::NCCALogger log;

class Foo
{
	public :
	Foo(int id):m_id(id){}
	void foo(const std::string  &a, const std::string &b)
	{
		while(1)
		log.logMessage("foo(str,str) %d ID %d value %s %s \n"
			,m_id,std::this_thread::get_id(),a.c_str(),b.c_str());
	}

	void foo(int a)
	{
		while(1)
		log.logMessage("foo(int) %d ID %d value %d \n"
			,m_id,std::this_thread::get_id(),a);
	}

	void foo(double a)
	{
		while(1)
		log.logMessage("foo(double) %d ID %d value %f\n"
			,m_id,std::this_thread::get_id(),a);
	}
	private :
		int m_id;

};

int main()
{
  std::vector<std::thread> threads;
  threads.reserve(6);
  log.setColour(nccalog::CYAN);
  log.logWarning("creating thread String Function\n");
  auto *pFoo=new Foo(10);
  Foo b(20);

	
	auto funca = std::bind( static_cast<void (Foo::*)( int )>(&Foo::foo),b,2);
	threads.emplace_back(funca);

	auto funcb = std::bind( static_cast<void (Foo::*)( int )>(&Foo::foo),pFoo,99);
	threads.emplace_back(funcb);
	
	auto funcc = std::bind( static_cast<void (Foo::*)( double )>(&Foo::foo),b,2.23);
	threads.emplace_back(funcc);

	auto funcd = std::bind( static_cast<void (Foo::*)( double )>(&Foo::foo),pFoo,9.9);
	threads.emplace_back(funcd);

  std::string sa="hello";
  std::string sb=" c++ 11 threads";
	auto funce = std::bind( static_cast<void (Foo::*)( const std::string &,const std::string & )>
	(&Foo::foo),b,sa,sb);
	threads.emplace_back(funce);
	auto funcf = std::bind( static_cast<void (Foo::*)( const std::string &,const std::string & )>
	(&Foo::foo),pFoo,sa,sb);
	threads.emplace_back(funcf);

  int i=0;
  for(auto& thread : threads)
  {
      log.setColour(nccalog::YELLOW);
      log.logWarning("Joining thread %d\n",i++);
      thread.join();
  }

  delete pFoo;
  return EXIT_SUCCESS;
}