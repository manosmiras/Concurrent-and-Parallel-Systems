#include <thread>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

vector<double> randoms;

// 10 million
const int ITERATIONS = 10000000;

// -10 million to 10 million
const double MIN_VAL = -10000000;
const double MAX_VAL = 10000000;

//const int NUM_THREADS = 4;

int current_min = MAX_VAL;
int current_max = MIN_VAL;

void generate_randoms()
{
	// Create a random engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	default_random_engine e(static_cast<unsigned int>(millis.count()));

	// Create a distribution
	uniform_real_distribution<double> distribution(MIN_VAL, MAX_VAL);

	// Iterate
	for (unsigned int i = 0; i < ITERATIONS; ++i)
	{
		randoms.push_back(distribution(e));
		//cout << distribution(e) << endl;
	}
	cout << "Finished generating random numbers" << endl;
}

void find_min_max(int start, int end)
{
	for (unsigned int i = start; i < end; i++)
	{
		if (randoms[i] < current_min)
			current_min = randoms[i];
		if (randoms[i] > current_max)
			current_max = randoms[i];
	}
}

int main()
{
	generate_randoms();

	// Get the start time
	auto start = system_clock::now();
	for (unsigned int num_threads = 0; num_threads <= 6; ++num_threads)
	{
		auto total_threads = static_cast<unsigned int>(pow(2.0, num_threads));
		cout << "Current thread count: " << total_threads << endl;
		vector<thread> threads;
		int search_space = randoms.size() / total_threads;
		for (unsigned int n = 0; n < total_threads; ++n)
		{
			int start_point = n * search_space;
			int end_point = (n + 1) * search_space;
			//cout << "Thread " << n << " is searching from " << start_point << " to " << end_point << endl;
			threads.push_back(thread(find_min_max, start_point, end_point));
		}
		// Join the threads
		for (auto &t : threads)
			t.join();
		// Get the end time
		auto end = system_clock::now();
		// Get the total time
		auto total = end - start;

		cout << "Min is: " << current_min << endl;
		cout << "Max is: " << current_max << endl;
		cout << "Time taken: " << duration_cast<milliseconds>(total).count() << " ms" << endl;
		cout << "===============================" << endl;

		int current_min = MAX_VAL;
		int current_max = MIN_VAL;
	}
	return 0;
}