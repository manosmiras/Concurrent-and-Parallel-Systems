#pragma once

#include <exception>
#include <stack>
#include <memory>
#include <mutex>

template<typename T>
class threadsafe_stack
{
private:
	std::stack<T> data;
	// The mutex to controll access
	mutable std::mutex mut;
public:
	// Constructor
	threadsafe_stack() { }
	// Copy constructor
	threadsafe_stack(const threadsafe_stack &other)
	{
		// Lock other stack in order to copy data
		std::lock_guard<std::mutex> lock(other.mut);
		data = other.data;
	}
	// Push method, adds to the stack
	void push(T value)
	{
		// Lock access to the object
		std::lock_guard<std::mutex> lock(mut);
		// Push value onto the internal stack
		data.push(value);
	}
	// Pop method, removes from the stack
	T pop()
	{
		// Lock acces to the object
		std::lock_guard<std::mutex> lock(mut);
		// Check if stack is empty
		if (data.empty())
			throw std::exception("Stack is empty");
		// Access value at the top of the stack.
		auto res = data.top();
		// Remove the top item from the stack
		data.pop();
		// Return resource
		return res;
	}
	// Checks if the stack is empty
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(mut);
		return data.empty();
	}
};