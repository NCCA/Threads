#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <string>
#include <functional>

#include "Logger.h"


class Foo
{
	public :
		Foo(int id):m_id(id){}
		void foo(const std::string  &a, const std::string &b)
		{
			while(1)
				nccalog::NCCALogger::instance().logMessage("foo(str,str) %d ID %d value %s %s \n"
				,m_id,std::this_thread::get_id(),a.c_str(),b.c_str());
		}

	void foo(int a)
	{
		while(1)
		nccalog::NCCALogger::instance().logMessage("foo(int) %d ID %d value %d \n"
		,m_id,std::this_thread::get_id(),a);
	}

	void foo(double a)
	{
		while(1)
			nccalog::NCCALogger::instance().logMessage("foo(double) %d ID %d value %f\n"
			,m_id,std::this_thread::get_id(),a);
	}
	private :
		int m_id;

};

int main()
{
	std::vector<std::thread> threads;
	threads.reserve(6);
	nccalog::NCCALogger::instance().setColour(nccalog::Colours::CYAN);
	nccalog::NCCALogger::instance().logWarning("creating thread String Function\n");
	std::shared_ptr<Foo> pFoo(new Foo(10));
	Foo b(20);


	auto funca = std::bind( static_cast<void (Foo::*)( int )>(&Foo::foo),b,2);
	threads.emplace_back(funca);

	auto funcb = std::bind( static_cast<void (Foo::*)( int )>(&Foo::foo),pFoo.get(),99);
	threads.emplace_back(funcb);

	auto funcc = std::bind( static_cast<void (Foo::*)( double )>(&Foo::foo),b,2.23);
	threads.emplace_back(funcc);

	auto funcd = std::bind( static_cast<void (Foo::*)( double )>(&Foo::foo),pFoo,9.9);
	threads.emplace_back(funcd);

	std::string sa="hello";
	std::string sb=" c++ 11 threads";
	auto funce = std::bind( static_cast<void (Foo::*)( const std::string &,const std::string & )>
	(&Foo::foo),b,sa,sb);
	threads.emplace_back(funce);
	auto funcf = std::bind( static_cast<void (Foo::*)( const std::string &,const std::string & )>
	(&Foo::foo),pFoo.get(),sa,sb);
	threads.emplace_back(funcf);

	std::for_each(std::begin(threads),std::end(threads),std::mem_fn(&std::thread::join));

	return EXIT_SUCCESS;
}