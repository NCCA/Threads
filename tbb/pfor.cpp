// clang++ -std=c++11 pfor.cpp -I$(HOME)/tbb/tbb2019_20190206oss/include -L/usr/local/lib -ltbb_debug

#include <tbb/tbb.h>
#include <pstl/execution>
#include <pstl/algorithm>
#include <vector>
#include <numeric>
#include <chrono>
#include <iostream>
#include <OpenCL/opencl.h>
#include "OpenCL.h"

template <typename Func>
long long timeFunc(Func f)
{
  auto begin = std::chrono::steady_clock::now();
  f();
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count();
}


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
  std::cout<<"TBB update took "<<std::chrono::duration_cast<std::chrono::microseconds> (end -begin).count()<<" uS ";

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
  std::cout<<"Normal For update took "<<std::chrono::duration_cast<std::chrono::microseconds> (end -begin).count()<<" uS ";

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
  std::cout<<"PSTL For update took "<<std::chrono::duration_cast<std::chrono::microseconds> (end -begin).count()<<" uS ";

}

void OpenCLFor(size_t _size)
{
  std::vector<int> numbers(_size);
  std::iota(std::begin(numbers),std::end(numbers),0);
  OpenCL cl("addOne.cl");
  cl.createKernel("addOne");
  cl_mem input;                       // device memory used for the input array
  input = clCreateBuffer(cl.getContext(),  CL_MEM_READ_WRITE,  sizeof(int) * _size, nullptr, nullptr);
  if (!input )
  {
      std::cerr<<"Error: Failed to allocate device memory!\n";
      exit(1);
  }
  auto err = clEnqueueWriteBuffer(cl.getCommands(), input, CL_TRUE, 0, sizeof(int) * _size, &numbers[0], 0, nullptr, nullptr);
  if (err != CL_SUCCESS)
  {
      std::cout<<"Error: Failed to write to source array!\n";
      exit(1);
  }

  err = 0;
  err  = clSetKernelArg(cl.getKernel(), 0, sizeof(cl_mem), &input);
  err |= clSetKernelArg(cl.getKernel(), 1, sizeof(unsigned int), &_size);
  if (err != CL_SUCCESS)
  {
      std::cerr<<"Error: Failed to set kernel arguments! "<< err<<'\n';
      exit(1);
  }

  size_t global;                      // global domain size for our calculation
  size_t local;                       // local domain size for our calculation

  err = clGetKernelWorkGroupInfo(cl.getKernel(), cl.getID(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, nullptr);
  //std::cout<<"work group size is "<<local<<"\n";
  if (err != CL_SUCCESS)
  {
      printf("Error: Failed to retrieve kernel work group info! %d\n", err);
      exit(1);
  }

  // Execute the kernel over the entire range of our 1d input data set
  // using the maximum number of work group items for this device
  //

  global = _size;
  auto begin = std::chrono::steady_clock::now();

  err = clEnqueueNDRangeKernel(cl.getCommands(), cl.getKernel(), 1, nullptr, &global, &local, 0, nullptr, nullptr);
  if (err)
  {
      std::cerr<<"Error: Failed to execute kernel! "<<err<<'\n';
      exit( EXIT_FAILURE);
  }

  // Wait for the command commands to get serviced before reading back results
  //
  clFinish(cl.getCommands());

  // Read back the results from the device to verify the output
  //
  err = clEnqueueReadBuffer( cl.getCommands(), input, CL_TRUE, 0, sizeof(int) * _size, &numbers[0], 0, nullptr, nullptr );
  if (err != CL_SUCCESS)
  {//
      printf("Error: Failed to read output array! %d\n", err);
      exit(1);
  }


  auto end = std::chrono::steady_clock::now();
  std::cout<<"OpenCL For update took "<<std::chrono::duration_cast<std::chrono::microseconds> (end -begin).count()<<" uS ";

}

int main()
{
  size_t size=(1024*1024)*60;
  std::cout<<"Data Size is "<<size<<'\n';

  auto time=timeFunc([&](){ NormalFor(size);});
  std::cout<<"Total Time "<<time<<" uS\n";

  time=timeFunc([&](){ TBBPFor(size);});
  std::cout<<"Total Time "<<time<<" uS\n";

  time=timeFunc([&](){ PSTLFor(size);});
  std::cout<<"Total Time "<<time<<" uS\n";

  time=timeFunc([&](){ OpenCLFor(size);});
  std::cout<<"Total Time "<<time<<" uS\n";


}



