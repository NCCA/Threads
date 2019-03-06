// clang++ -std=c++11 pfor.cpp -I$(HOME)/tbb/tbb2019_20190206oss/include -L/usr/local/lib -ltbb_debug

#include <tbb/tbb.h>
#include <pstl/execution>
#include <pstl/algorithm>
#include <vector>
#include <numeric>
#include <chrono>
#include <iostream>

void TBBPFor(size_t _size)
{
  std::vector<int> numbers(_size);
  std::iota(std::begin(numbers),std::end(numbers),0);

  auto begin = std::chrono::steady_clock::now();

  tbb::parallel_for( tbb::blocked_range<size_t>(0,16),
    [&numbers]( tbb::blocked_range<size_t>& r)
    {
      for(size_t i=r.begin(); i!=r.end(); ++i)
          numbers[i]+=1;
    }
 );
  auto end = std::chrono::steady_clock::now();
  std::cout<<"TBB update took "<<std::chrono::duration_cast<std::chrono::microseconds> (end -begin).count()<<" uS\n";

}

void NormalFor(size_t _size)
{
  std::vector<int> numbers(_size);
  std::iota(std::begin(numbers),std::end(numbers),0);

  auto begin = std::chrono::steady_clock::now();

  std::for_each( std::begin(numbers),std::end(numbers),
    [](int &n){ n+=1; }
 );
  auto end = std::chrono::steady_clock::now();
  std::cout<<"Normal For update took "<<std::chrono::duration_cast<std::chrono::microseconds> (end -begin).count()<<" uS\n";

}

void PSTLFor(size_t _size)
{
  std::vector<int> numbers(_size);
  std::iota(std::begin(numbers),std::end(numbers),0);

  auto begin = std::chrono::steady_clock::now();

  std::for_each( pstl::execution::par_unseq,std::begin(numbers),std::end(numbers),
    [](int &n){ n+=1; }
 );
  auto end = std::chrono::steady_clock::now();
  std::cout<<"PSTL For update took "<<std::chrono::duration_cast<std::chrono::microseconds> (end -begin).count()<<" uS\n";

}



int main()
{
  size_t size=1000000;
  NormalFor(size);
  TBBPFor(size);
  PSTLFor(size);
}



