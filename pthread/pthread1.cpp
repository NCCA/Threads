#include <iostream>
#include <cstdlib>
#include <pthread.h>

void *threadFunc(void *arg)
{
	for(int i=0; i<10; ++i)
	std::cout<<"thread func "<<i<<"\n";
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
		pthread_join(threadID[i],0);
	}
}








