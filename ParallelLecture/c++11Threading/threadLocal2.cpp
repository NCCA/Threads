#include <thread>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "Logger.h"


class Counter
{
	public :
		void increment() { ++m_count; }
		~Counter()
		{
			nccalog::NCCALogger::instance().logWarning("Thread %d called %d times \n",std::this_thread::get_id() ,m_count	);
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

	for_each(std::begin(threads),std::end(threads),std::mem_fn(&std::thread::join));

}	