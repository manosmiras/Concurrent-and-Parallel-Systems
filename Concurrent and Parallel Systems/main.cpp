#include <thread>
#include <iostream>

using namespace std;

void hello_world()
{
	cout << "Hello from thread " << this_thread::get_id() << endl;
}

int main()
{
	// Create a new thread
	thread t(hello_world);
	// Wait for thread to finish
	t.join();


	return 0;
}