#include <iostream>
#include <thread>
#include <cstdlib>
#include "Logger.h"

nccalog::NCCALogger log;


void hello()
{
	log.setColour(nccalog::RED);
	for(int i=0; i<10; ++i)
		log.logMessage("thread func %d\n",i);
}


int main()
{
	unsigned long const nThreads=std::thread::hardware_concurrency();
	std::cout<<"num threads "<<nThreads<<"\n";
	std::thread t(hello);
	t.join();

	return EXIT_SUCCESS;
}