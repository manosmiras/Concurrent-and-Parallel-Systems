#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <iostream>
using namespace std;
using namespace chrono;
vector<unsigned int> generate_values(unsigned int size)
{
	// Create random engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	default_random_engine e(static_cast<unsigned int> (millis.count()));
	// Generate random numbers
	vector<unsigned int> data;

	for (unsigned int i = 0; i < size; ++i)
	{
		data.push_back(e());
	}
	return data;
}

void bubble_sort(vector<unsigned int> &values)
{
	for (size_t count = 1; count < values.size(); ++count)
	{
		for (size_t i = 0; i < (values.size() - 1); ++i)
		{
			if (values[i] > values[i + 1])
			{
				unsigned int temp = values[i];
				values[i] = values[i + 1];
				values[i + 1] = temp;
			}
		}
	}
}

int main()
{
	// Create results file
	ofstream results("bubble.csv", ofstream::out);
	// Gather results for 2^8 to 2^16 results
	for (size_t size = 8; size <= 16; ++size)
	{
		// Output data size
		results << pow(2, size) << ", ";
		// Gather 100 results
		for (size_t i = 0; i < 10; ++i)
		{
			// Generate vector of random values
			cout << "Generating " << i << " for " << pow(2, size) << " values" << endl;
			auto data = generate_values(static_cast<unsigned int>(pow(2, size)));

			// Sort the vector
			cout << "Sorting" << endl;

			auto start = system_clock::now();
			bubble_sort(data);
			auto end = system_clock::now();
			auto total = duration_cast<milliseconds>(end - start).count();
			// Output time
			results << total << ",";
		}
		results << endl;
	}
	results.close();

	return 0;
}