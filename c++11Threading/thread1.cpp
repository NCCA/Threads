#include <iostream>
#include <thread>
#include <cstdlib>


void hello()
{
	std::cout<<"hello from thread\n";
}


int main()
{
	auto nThreads=std::thread::hardware_concurrency();
	std::cout<<"num threads "<<nThreads<<'\n';
	std::thread t(hello);
	t.join();

	return EXIT_SUCCESS;
}