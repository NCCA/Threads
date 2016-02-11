#include <thread>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "Logger.h"

nccalog::NCCALogger log;

class Counter
{
	public :
     void increment() { ++m_count; }
     ~Counter()
     {
         log.logWarning("Thread %d called %d times \n",
         std::this_thread::get_id() ,m_count	);
     }
    private :
         unsigned int m_count = 0;


};


thread_local Counter c;

void threadTask()
{
    c.increment();
}

int main()
{
	std::vector<std::thread> threads;
  for(int i=0; i<10; ++i)
  	threads.push_back(std::thread(threadTask));
  for(auto& thread : threads)
    {
        thread.join();
    }

}	