#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Logger.h"


void task()
{
    nccalog::NCCALogger::instance().setColour(nccalog::Colours::RED);
    for(int i=0; i<4; ++i)
    nccalog::NCCALogger::instance().logMessage(  "task %x\n" ,std::this_thread::get_id() );
}

int main(){
    std::vector<std::thread> threads;

    for(int i = 0; i < 5; ++i)
    {
        nccalog::NCCALogger::instance().setColour(nccalog::Colours::CYAN);
        nccalog::NCCALogger::instance().logWarning("creating thread %d\n",i);
        threads.push_back(std::thread(task));
    }
    int i=0;
    for(auto& thread : threads)
    {
        nccalog::NCCALogger::instance().setColour(nccalog::Colours::YELLOW);
        nccalog::NCCALogger::instance().logWarning("Joining thread %d\n",i++);
        thread.join();
    }

    return EXIT_SUCCESS;
}