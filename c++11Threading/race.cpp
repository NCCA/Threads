#include <iostream>
#include <random>
#include <thread>
#include <chrono>

int counter=0;

void run(int runs)
{
  std::cout<<"Thread " << std::this_thread::get_id()<<" is running\n";
  for(int i=0; i<runs; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%4));
    counter++;
  }
}


int main(int argc, char **argv)
{
  int N = atoi(argv[1]);
  int runs= atoi(argv[2]);
  std::thread *t[N];
  for(int i=0; i<N; ++i)
  {
    t[i]=new std::thread(run,runs);
  }
  for(int i=0; i<N; ++i)
  {
    t[i]->join();
  }
  std::cout<<counter<<'\n';


}