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
	printf("thread function %d %c \n",args->arg1,args->arg2);
	int ret=args->arg1+10;
	pthread_exit(((void*)(ret)));
}


int main()
{
	pthread_t threadID[4];
	struct argStruct args[4];

	for(int i=0; i<4; ++i)
	{
		args[i].arg1=i;
		args[i].arg2='a'+i;
		pthread_create(&threadID[i],0,threadFunc,(void *)&args[i]);
	}
	// now join

  int ret;
	for(int i=0; i<4; ++i)
	{
		printf("join\n");
		pthread_join(threadID[i],(void **)&ret);
		printf("return %d\n",ret);
	}
}