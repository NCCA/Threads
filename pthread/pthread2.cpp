#include <iostream>
#include <cstdlib>
#include <pthread.h>

struct argStruct
{
	int arg1;
	char arg2;
};


void *threadFunc(void *arg)
{
	struct argStruct *args = (argStruct *)arg;
/*	std::cout<<"thread func "<<std::endl;
	std::cout<<"Arg 1 "<<args->arg1<<std::endl;
	std::cout<<"Arg 2 "<<args->arg2<<std::endl;
	*/
	printf("thread function %d %c \n",args->arg1,args->arg2);
	int ret=args->arg1*2;
}


int main()
{
	pthread_t threadID[4];
	struct argStruct args;

	for(int i=0; i<4; ++i)
	{
		args.arg1=i;
		args.arg2='a'+i;
		pthread_create(&threadID[i],0,threadFunc,(void *)&args);
	}
	// now join

	int retval;
	for(int i=0; i<4; ++i)
	{
		pthread_join(threadID[i],0);
	}
}