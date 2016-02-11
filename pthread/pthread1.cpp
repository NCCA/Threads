#include <iostream>
#include <cstdlib>
#include <pthread.h>

void *threadFunc(void *arg)
{
	for(int i=0; i<10000; ++i)
	{
		std::cout<<"thread func "<<i<<" ";
		std::cout.flush();
	}
std::cout<<std::endl;
return 0;
}


int main()
{
	pthread_t threadID[8];
	for(int i=0; i<8; ++i)
	{
		pthread_create(&threadID[i],0,threadFunc,0);
	}

	// now join
	for(int i=0; i<4; ++i)
	{
		std::cout<<"*************************\n";
		pthread_join(threadID[i],0);
	}
	std::cout<<"###########################\n";
}








