#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>

std::mutex g_print;
void task()
{
    g_print.lock();
    for(int i=0; i<20; ++i)
    std::cout << "task " << std::this_thread::get_id() << std::endl;
    g_print.unlock();
}

int main(){
    std::vector<std::thread> threads;

    for(int i = 0; i < 5; ++i)
    {
        threads.push_back(std::thread(task));
    }

    for(auto& thread : threads)
    {
        thread.join();
    }

    return EXIT_SUCCESS;
}