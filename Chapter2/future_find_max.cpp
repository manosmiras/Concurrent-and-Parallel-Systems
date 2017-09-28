#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <future>
#include <iostream>

using namespace std;
using namespace chrono;

unsigned int find_max(const vector<unsigned int> &data, unsigned int start, unsigned int end)
{
	// Set max initially to 0
	unsigned int max = 0;

	// Iterate across vector from start to end pos, setting max accordingly
	for (size_t i = start; i < end; ++i)
	{
		if (data.at(i) > max)
			max = data.at(i);
	}

	return max;
}

//int main()
//{
//	// Get number of supported threads
//	auto num_threads = thread::hardware_concurrency();
//
//	// Create a vector with 2^24 random values
//	vector<unsigned int> values;
//	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//	default_random_engine e(static_cast<unsigned int>(millis.count()));
//
//	for (size_t i = 0; i < pow(2, 24); ++i)
//	{
//		values.push_back(e());
//	}
//
//	// Create num threads - 1 futures
//	vector<future<unsigned int>> futures;
//	auto range = static_cast<unsigned int>(pow(2, 24) / num_threads);
//	for (size_t i = 0; i < num_threads - 1; ++i)
//	{
//		futures.push_back(async(find_max, ref(values), i * range, (i + 1) * range));
//	}
//	// Main application thread will process the end of the list
//	auto max = find_max(values, (num_threads - 1) * range, num_threads * range);
//
//	for (auto &f : futures)
//	{
//		auto result = f.get();
//		if (result > max)
//		{
//			max = result;
//		}
//	}
//	cout << "Maximum value found: " << max << endl;
//
//	return 0;
//}