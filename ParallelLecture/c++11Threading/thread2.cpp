#include <thread>
#include <iostream>
#include <cstdlib>



class Task
{
	public :

		Task(){m_id=99;}
		Task(int _t) : m_id(_t){;}

		void operator()() const
		{
			std::cout<<"class operator called "<<m_id<<"\n";
		}

	private :
		int m_id;
		

};


int main()
{
	Task t;
	unsigned long const nThreads=std::thread::hardware_concurrency();
	std::cout<<"num threads "<<nThreads<<"\n";
	std::thread thread( (Task(2)));
	thread.join();

	return EXIT_SUCCESS;
}











