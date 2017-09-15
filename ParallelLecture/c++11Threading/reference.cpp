#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <functional>

#include "Logger.h"

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
  nccalog::NCCALogger::instance().setColour(nccalog::Colours::CYAN);
  nccalog::NCCALogger::instance().logWarning("creating thread String Function\n");
  Foo *pFoo=new Foo(50);
  Foo b(99);

  int value1=10;
  int value2=20;	
  auto funca = std::bind( static_cast<void (Foo::*)( int &)>(&Foo::mutate),b,std::ref(value1));
  threads.emplace_back(funca);

  auto funcb = std::bind( static_cast<void (Foo::*)( int &)>(&Foo::mutate),pFoo,std::ref(value2));
  threads.emplace_back(funcb);


  nccalog::NCCALogger::instance().setColour(nccalog::Colours::YELLOW);
  nccalog::NCCALogger::instance().logWarning("Joining threads \n");
  std::for_each(std::begin(threads),std::end(threads),std::mem_fn(&std::thread::join));
  nccalog::NCCALogger::instance().logError("Value a %d Value b %d \n",value1,value2);

return EXIT_SUCCESS;
}