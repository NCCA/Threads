#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Logger.h"

nccalog::NCCALogger log;

void task()
{
    log.setColour(nccalog::RED);
    for(int i=0; i<4; ++i)
        log.logMessage(  "task %x\n" ,std::this_thread::get_id() );
}

int main(){
    std::vector<std::thread> threads;

    for(int i = 0; i < 5; ++i)
    {
        log.setColour(nccalog::CYAN);
        log.logWarning("creating thread %d\n",i);
        threads.push_back(std::thread(task));
    }
    int i=0;
    for(auto& thread : threads)
    {
        log.setColour(nccalog::YELLOW);
        log.logWarning("Joining thread %d\n",i++);
        thread.join();
    }

    return EXIT_SUCCESS;
}