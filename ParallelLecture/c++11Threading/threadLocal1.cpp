#include <thread>
#include <iostream>
#include <cstdlib>
// needs g++ not clang

thread_local int i=0;

void foo(int*p)
{
    *p=42;
}

int main()
{
  i=9;
  std::thread t(foo,&i);
  t.join();
  std::cout<<i<<'\n';
}