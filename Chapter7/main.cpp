// Initialise OpenCL
#include <CL\cl.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

void initialise_opencl(std::vector<cl_platform_id> &platforms, std::vector<cl_device_id> &devices, cl_context &context, cl_command_queue &cmd_queue)
{
	// Status of OpenCL calls
	cl_int status;
	// Get te number of platforms
	cl_uint num_platforms;
	status = clGetPlatformIDs(0, nullptr, &num_platforms);
	// Resize vector to store platforms
	platforms.resize(num_platforms);
	// Fill in platform vector
	status = clGetPlatformIDs(num_platforms, &platforms[0], nullptr);

	// Assume platform 0 is the one we want to use
	// Get devices for platform 0
	cl_uint num_devices;
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
	// Resize vector to store devices
	devices.resize(num_devices);
	// Fill in devices vector
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, num_devices, &devices[0], nullptr);
	// Create a context
	context = clCreateContext(nullptr, num_devices, &devices[0], nullptr, nullptr, &status);

	// Create a command queue
	cmd_queue = clCreateCommandQueue(context, devices[0], 0, &status);
}

// Helper method to print OpenCL device info
void print_opencl_info(std::vector<cl_device_id> &devices)
{
	// Buffers for device name and vendor
	char device_name[1024], vendor[1024];
	// Declare other necessary variables
	cl_uint num_cores;
	cl_long memory;
	cl_uint clock_freq;
	cl_bool available;

	// Iterate through each device in vector and display information
	for (auto &d : devices)
	{
		// Get info for device
		clGetDeviceInfo(d, CL_DEVICE_NAME, 1024, device_name, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_VENDOR, 1024, vendor, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num_cores, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_long), &memory, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &clock_freq, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_AVAILABLE, sizeof(cl_bool), &available, nullptr);

		// Print info
		std::cout << "Device: " << device_name << std::endl;
		std::cout << "Vendor: " << vendor << std::endl;
		std::cout << "Cores: " << num_cores << std::endl;
		std::cout << "Memory: " << memory / (1024 * 1024) << "MB" << std::endl;
		std::cout << "Clock freq: " << clock_freq << "MHz" << std::endl;
		std::cout << "Available: " << available << std::endl;
		std::cout << "*************************************" << std::endl << std::endl;
	}
}

// Loads an OpenCL program
cl_program load_program(const std::string &filename, cl_context &context, cl_device_id &device, cl_int num_devices)
{
	// Status of OpenCL calls
	cl_int status;

	// Create and compile program
	// Read in kernel file
	std::ifstream input(filename, std::ifstream::in);
	std::stringstream buffer;
	buffer << input.rdbuf();
	// Get the character array of the file contents
	auto file_contents = buffer.str();
	auto char_contents = file_contents.c_str();

	// Create program object
	auto program = clCreateProgramWithSource(context, 1, &char_contents, nullptr, &status);
	// Compile / build program
	status = clBuildProgram(program, num_devices, &device, nullptr, nullptr, nullptr);

	// Check if compiled
	if (status != CL_SUCCESS)
	{
		// Error building - get log
		size_t length;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &length);
		char *log = new char[length];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, length, log, &length);
		// Print log
		std::cout << log << std::endl;
		delete[] log;
	}

	// Return program object
	return program;
}

int main()
{
	// Status of OpenCL calls
	cl_int status;

	// Initialise OpenCL
	std::vector<cl_platform_id> platforms;
	std::vector<cl_device_id> devices;
	cl_context context;
	cl_command_queue cmd_queue;
	initialise_opencl(platforms, devices, context, cmd_queue);

	// Print info
	print_opencl_info(devices);


	// Load program
	auto program = load_program("kernel.cl", context, devices[0], devices.size());

	// Create the kernel
	auto kernel = clCreateKernel(program, "vecadd", &status);

	// ======================================

	// Number of elements and size of buffer on GPU
	const unsigned int elements = 2048;
	const unsigned int data_size = sizeof(int) * elements;

	// Host data - stored in main memory
	std::array<int, elements> A;
	std::array<int, elements> B;
	std::array<int, elements> C;

	// Initialise input data
	for (size_t i = 0; i < elements; i++)
	{
		A[i] = B[i] = i;
	}

	// Create device buffer - stored on GPU
	cl_mem buffer_A; // Input array on the device
	cl_mem buffer_B; // Input array on the device
	cl_mem buffer_C; // Output array on the device
					 // Allocate buffer size
	buffer_A = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, nullptr, &status);
	buffer_B = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, nullptr, &status);
	buffer_C = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, nullptr, &status);

	status = clEnqueueWriteBuffer(cmd_queue, buffer_A, CL_FALSE, 0, data_size, A.data(), 0, nullptr, nullptr);
	status = clEnqueueWriteBuffer(cmd_queue, buffer_B, CL_FALSE, 0, data_size, B.data(), 0, nullptr, nullptr);

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_A);
	status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_B);
	status |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_C);

	// ======================================

	// Configure the work dimensions - 1D of elements
	std::array<size_t, 1> global_work_size = { elements };
	// Enqueue the kernel for execution
	status = clEnqueueNDRangeKernel(cmd_queue, kernel, 1, nullptr, global_work_size.data(), nullptr, 0, nullptr, nullptr);
	// Read the output buffer from the GPU to main memory
	clEnqueueReadBuffer(cmd_queue, buffer_C, CL_TRUE, 0, data_size, C.data(), 0, nullptr, nullptr);

	// Verify the output
	auto result = true;
	int i = 0;
	// Iterate through each value in result array
	for (auto &e : C)
	{
		// Check value
		if (e != i + i)
		{
			result = false;
			break;
		}
		++i;
	}
	// Check if result is true and display accordingly
	if (result)
		std::cout << "Output is correct" << std::endl;
	else
		std::cout << "Output is incorrect" << std::endl;

	// Free OpenCL resources
	clReleaseMemObject(buffer_A);
	clReleaseMemObject(buffer_B);
	clReleaseMemObject(buffer_C);

	clReleaseCommandQueue(cmd_queue);
	clReleaseContext(context);
	clReleaseKernel(kernel);
	clReleaseProgram(program);

	return 0;
}