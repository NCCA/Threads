#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include "MutexLocker.h"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


void *threadFunc(void *arg)
{

	MutexLocker lock(&mutex);	
	for(int i=0; i<4; ++i)
		std::cout<<pthread_self()<<" thread func "<<i<<"\n";
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
		pthread_detach(threadID[i]);
	}
}








