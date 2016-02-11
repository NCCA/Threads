#include <thread>
#include <atomic>
#include <iostream>
#include <vector>

class Counter 
{
  public :

    Counter() : m_value(0) {}

    void increment(){ ++m_value;}

    void decrement(){ --m_value;}

    int get()
    {
    return m_value.load();
    }

  public :
    std::atomic<int> m_value;

};

int main()
{
  Counter counter;

  std::vector<std::thread> threads;
  for(int i = 0; i < 10; ++i)
  {
    threads.push_back(std::thread([&counter]()
    {
      for(int i = 0; i < 500; ++i)
      {
        counter.increment();
      }
    }));
  }

  for(auto& thread : threads)
  {
    thread.join();
  }

 std::cout << counter.get() << std::endl;

 return 0;
}