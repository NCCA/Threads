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
	
	void mutate(int &io_b)
	{
		io_b+=m_id;
	}
	
	private :
		int m_id;

};

int main()
{
  std::vector<std::thread> threads;
  threads.reserve(2);
  log.setColour(nccalog::CYAN);
  log.logWarning("creating thread String Function\n");
  Foo *pFoo=new Foo(50);
  Foo b(99);

	int value1=10;
	int value2=20;	
	auto funca = std::bind( static_cast<void (Foo::*)( int &)>(&Foo::mutate),b,std::ref(value1));
	threads.emplace_back(funca);

	auto funcb = std::bind( static_cast<void (Foo::*)( int &)>(&Foo::mutate),pFoo,std::ref(value2));
	threads.emplace_back(funcb);
	

  int i=0;
  for(auto& thread : threads)
  {
      log.setColour(nccalog::YELLOW);
      log.logWarning("Joining thread %d\n",i++);
      thread.join();
  }
  log.logError("Value a %d Value b %d \n",value1,value2);

  return EXIT_SUCCESS;
}