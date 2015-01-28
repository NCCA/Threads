#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

char *sharedMem;
const static int SIZE=20;

void *starFillerThread(void *arg)
{
	while(1)
	{
	printf("Star Filler\n");
	for(int i=0; i<SIZE; ++i)
		sharedMem[i]='*';
	sleep(2);
	}
}

void *hashFillerThread(void *arg)
{
	while(1)
	{
	printf("hash filler\n");
	for(int i=0; i<SIZE; ++i)
		sharedMem[i]='#';
	sleep(2);
	}
}


void *consumerThread(void *arg)
{
		while(1)
		{
		printf("Consumer\n");
		for(int i=0; i<SIZE; ++i)
			printf("%c",sharedMem[i]);
		printf("\n");
		sleep(2);
		}
}

int main()
{
	sharedMem = new char[SIZE];
	pthread_t threadID[3];

	pthread_create(&threadID[0],0,starFillerThread,0);
	pthread_create(&threadID[1],0,hashFillerThread,0);
	pthread_create(&threadID[2],0,consumerThread,0);

	pthread_join(threadID[0],0);
	pthread_join(threadID[1],0);
	pthread_join(threadID[2],0);

	while(1){ printf("\n");}




}