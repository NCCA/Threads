#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

char *sharedMem;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
const static int SIZE=20;

void *starFillerThread(void *arg)
{

	while(1)
	{
	pthread_mutex_lock (&mutex);
	printf("Star Filler\n");
	for(int i=0; i<SIZE; ++i)
		sharedMem[i]='*';
	pthread_mutex_unlock(&mutex);
	sleep(2);
	}
}

void *hashFillerThread(void *arg)
{
	while(1)
	{
	pthread_mutex_lock (&mutex);
	printf("hash filler\n");
	for(int i=0; i<SIZE; ++i)
		sharedMem[i]='#';
	pthread_mutex_unlock (&mutex);
	sleep(2);
	}
}


void *consumerThread(void *arg)
{
		while(1)
		{
		pthread_mutex_lock (&mutex);
		printf("Consumer\n");
		for(int i=0; i<SIZE; ++i)
			printf("%c",sharedMem[i]);
		pthread_mutex_unlock (&mutex);
		printf("\n");
		sleep(2);
		}
}

int main()
{
	sharedMem = new char[SIZE];
	pthread_t threadID[3];
  pthread_mutex_init(&mutex, 0);

	pthread_create(&threadID[0],0,starFillerThread,0);
	pthread_create(&threadID[1],0,hashFillerThread,0);
	pthread_create(&threadID[2],0,consumerThread,0);

	pthread_join(threadID[0],0);
	pthread_join(threadID[1],0);
	pthread_join(threadID[2],0);
	//while(1){ printf("\n");}

}