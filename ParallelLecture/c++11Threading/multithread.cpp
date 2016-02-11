#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>

void task()
{
    for(int i=0; i<20; ++i)
    std::cout << "task " << std::this_thread::get_id() << std::endl;
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