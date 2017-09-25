#ifndef GUARDED_H
#define GUARDED_H

#include <mutex>

class guarded
{
private:
	std::mutex mut;
	int value;
public:
	guarded() : value(0) { }
	~guarded() { }
	int get_value() const { return value; }
	void increment();
};

#endif // GUARDED_H