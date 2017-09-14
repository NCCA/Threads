#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>
#include <future>
#include <numeric>


const static unsigned int size=100000000;

int sumVect(const std::vector<int>& v)
{
	std::cout<<"sumVect\n";
	int sum=0;
	for(auto i : v)
		sum += i;
	return sum;
}

int sumVectLambda(const std::vector<int>& v)
{
	std::cout<<"sumLambda\n";

	int sum=0;
	for_each(std::begin(v),std::end(v), // algorithm + lambda
	 [&sum](int x) {sum += x; });
	return sum;
}
int main()
{
	std::vector <int> data(size);
	std::iota(std::begin(data),std::end(data),0);

	auto res1 = std::async(sumVect,data);
 	auto res2 = std::async(sumVectLambda,data);
	std::cout<<"start timer\n";
	auto t0 = std::chrono::high_resolution_clock::now();
	auto a=res1.get();
	auto b=res2.get();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds totalMs = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);



	std::cout<<"using standard vector "<<a<<" "<<b<<" took "<<totalMs.count()<<" Ms \n";
	return EXIT_SUCCESS;
}