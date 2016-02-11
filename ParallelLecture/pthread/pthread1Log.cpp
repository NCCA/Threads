#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include "Logger.h"

nccalog::NCCALogger log;

void *threadFunc(void *arg)
{
	log.setColour(nccalog::RED);
	for(int i=0; i<10; ++i)
	log.logMessage("thread func %d\n",i);
}


int main()
{
	pthread_t threadID[4];
	for(int i=0; i<4; ++i)
	{
		pthread_create(&threadID[i],0,threadFunc,0);
	}
	// now join
	for(int i=0; i<4; ++i)
	{
		log.setColour(nccalog::YELLOW);
		log.logMessage("Joining thread %d\n",i);
		pthread_join(threadID[i],0);
	}
}








