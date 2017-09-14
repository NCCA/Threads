#include <iostream>
#include <thread>
#include <cstdlib>
#include "Logger.h"



void hello()
{
	nccalog::NCCALogger::instance().setColour(nccalog::Colours::RED);
	for(int i=0; i<10; ++i)
		nccalog::NCCALogger::instance().logMessage("thread func %d\n",i);
}


int main()
{
	unsigned long const nThreads=std::thread::hardware_concurrency();
	std::cout<<"num threads "<<nThreads<<"\n";
	std::thread t(hello);
	t.join();

	return EXIT_SUCCESS;
}