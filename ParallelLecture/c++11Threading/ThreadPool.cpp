#include <iostream>
#include <cstdlib>
#include "ThreadPool.h"

void f1(){std::cout<<"a\n";}
void f2(){std::cout<<"b\n";}

int main(int argc, char* argv[])
{
ThreadPool tp(2);

for(int i=0; i<10; ++i)
{
	tp.enqueue(&f1);
	tp.enqueue(&f2);
}

std::this_thread::sleep_for(std::chrono::seconds(2));

	return EXIT_SUCCESS;
}