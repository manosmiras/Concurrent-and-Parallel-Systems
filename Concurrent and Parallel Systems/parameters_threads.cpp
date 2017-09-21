#include <thread>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int num_threads = 100;

void task(int n, int val)
{
	cout << "Thread: " << n << " Random Value : " << val << endl;
}

//int main()
//{
//	// Create random
//	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//
//	default_random_engine e(static_cast<uint32_t>(millis.count()));
//
//	// Create 100 threads in a vector
//	vector<thread> threads;
//	for (size_t i = 0; i < num_threads; i++)
//	{
//		threads.push_back(thread(task, i, e()));
//	}
//
//	// Join the threads
//	for (thread &t : threads)
//	{
//		t.join();
//	}
//
//	return 0;
//}