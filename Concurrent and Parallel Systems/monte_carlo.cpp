#include <thread>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

void monte_carlo_pi(unsigned int iterations)
{
	// Create a rrandom engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	default_random_engine e(static_cast<uint32_t>(millis.count()));

	// Create a distribution - we want doubles between 0.0 and 1.0
	uniform_real_distribution<double> distribution(0.0, 1.0);

	// Keep track of number of points in circle
	unsigned int in_circle = 0;
	// Iterate
	for (unsigned int i = 0; i < iterations; ++i)
	{
		// Generate random point
		auto x = distribution(e);
		auto y = distribution(e);
		// Get length of vector defined - use Pythagoras
		auto length = sqrt((x * x) + (y * y));
		// Check if in circle
		if (length <= 1.0)
			++in_circle;
	}
	// Calculate pi
	auto pi = (4.0 * in_circle) / static_cast<double>(iterations);
}