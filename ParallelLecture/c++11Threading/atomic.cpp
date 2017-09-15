#include <thread>
#include <atomic>
#include <iostream>
#include <vector>

class Counter 
{
  public :

    Counter() =default;

    void increment(){ ++m_value;}

    void decrement(){ --m_value;}

    int get()
    {
      return m_value.load();
    }

  public :
    std::atomic<int> m_value={0};

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
      }
    ));
  }

  std::for_each(std::begin(threads),std::end(threads),std::mem_fn(&std::thread::join));
  

  std::cout << counter.get() << '\n';

  return 0;
}