#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>

std::mutex g_print;
void task()
{
    g_print.lock();
    std::cout << "task id=" << std::this_thread::get_id() << '\n';
    g_print.unlock();
}

int main()
{
    std::vector<std::thread> threads(5);

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