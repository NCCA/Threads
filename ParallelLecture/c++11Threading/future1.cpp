#include <thread>
#include <future>
#include <iostream>

void func(std::promise<int> && p) 
{
	p.set_value(99);
}


int main()
{
	std::promise<int> p;
	auto f = p.get_future();
	std::thread t(&func, std::move(p));
	t.join();
	int i = f.get();
	std::cout<<i<<'\n';
}