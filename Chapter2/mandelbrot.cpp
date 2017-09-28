#include <vector>
#include <future>
#include <iostream>
#include "FreeImage\FreeImage.h"
using namespace std;

// Number of iterations to perform to find pixel value
const unsigned int max_iterations = 1000;
// Dimension of the image (in pixels) to generate
const unsigned int dim = 8192;

// Mandelbrot dimensions are ([-2.1, 1.0], [-1.3, 1.3])
const double xmin = -2.1;
const double xmax = 1.0;
const double ymin = -1.3;
const double ymax = 1.3;

// The conversion from Mandelbrot coordinate to image coordinate
const double integral_x = (xmax - xmin) / static_cast<double>(dim);
const double integral_y = (ymax - ymin) / static_cast<double>(dim);

vector<double> mandelbrot(unsigned int start_y, unsigned int end_y)
{
	// Declare values we will use
	double x, y, x1, y1, xx = 0.0;
	unsigned int loop_count = 0;
	// Where to store the results
	vector<double> results;

	// Loop through each line
	y = ymin + (start_y * integral_y);
	for (size_t y_coord = start_y; y_coord < end_y; ++y_coord)
	{
		x = xmin;
		// Loop through each pixel on the line
		for (size_t x_coord = 0; x_coord < dim; ++x_coord)
		{
			x1 = 0.0, y1 = 0.0;
			loop_count = 0;
			// Calculate the mandelbrot value
			while (loop_count < max_iterations && sqrt((x1 * x1) + (y1 * y1)) < 2.0)
			{
				++loop_count;
				xx = (x1 * x1) - (y1 * y1) + x;
				y1 = 2 * x1 * y1 + y;
				x1 = xx;
			}
			// Get value where loop completed
			auto val = static_cast<double>(loop_count) / static_cast<double>(max_iterations);
			// Push this value onto the vector
			results.push_back(val);
			// Increase x based on integral
			x += integral_x;
		}
		// Increase y based on integral
		y += integral_y;
	}
	// Return vector
	return results;
}

int main()
{
	//FreeImage_Initialise();
	auto num_threads = thread::hardware_concurrency();

	// Determine strip height
	auto strip_height = dim / num_threads;

	// Create futures
	vector<future<vector<double>>> futures;

	for (size_t i = 0; i < num_threads; ++i)
	{
		unsigned int start = i * strip_height;
		unsigned int end = (i + 1) * strip_height;
		// Range is used to determine number of values to process
		futures.push_back(async(mandelbrot, start, end));
	}

	// Vector to store results
	vector<vector<double>> results;
	// Get results
	for (auto &f : futures)
	{
		results.push_back(f.get());
	}
	
	for (auto &result : results)
	{
		
	}

	return 0;
}