#pragma once
#include <vector>

using namespace std;

class RollingSampleBuffer {
private:
	vector<float> _buffer;
	int _index;
	int _count;
public:
	int Capacity;
	int Count;

	RollingSampleBuffer() = default;
	RollingSampleBuffer(int size);
	~RollingSampleBuffer() = default;

	float Add(float value);

	float Get(int i);

	float operator[](int i);
};


