#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include  <unistd.h>

std::mutex g_print;
using namespace std::chrono_literals;

void task()
{
    while(1)
    {
        g_print.lock();
        std::cout << "task id=" << std::this_thread::get_id() << '\n';
        g_print.unlock();
        std::this_thread::sleep_for(1s);
    }
}

int main()
{
    std::vector<std::thread> threads(15);

    for(auto &t : threads)
    {
        t=std::thread(task);
    }

    for(auto& thread : threads)
    {
        thread.join();
    }

    return EXIT_SUCCESS;
}