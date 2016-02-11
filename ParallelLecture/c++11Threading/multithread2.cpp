#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <future>

std::mutex g_print;
void task()
{
    g_print.lock();
    std::cout << "task " << std::this_thread::get_id() << std::endl;
    g_print.unlock();
}

int main()
{
    unsigned int nthreads = std::thread::hardware_concurrency();
    std::cout<<nthreads<<"\n";
    std::vector<std::future<void>> threads;

    for(int i = 0; i < nthreads; ++i)
    {
       threads.push_back(std::async(task));
    }

    for(auto& thread : threads)
    {
        thread.get();
    }

    return EXIT_SUCCESS;
}