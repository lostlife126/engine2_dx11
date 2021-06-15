#pragma once

#include <sstream>
#include <vector>

template <typename T>
struct Vect3
{
	union
	{
		T data[3];
		struct
		{
			T x;
			T y;
			T z;
		};
	};
	T w;

	Vect3() :x(0), y(0), z(0), w (1) {}
	Vect3(T v) :x(v), y(v), z(v), w(1) {}
	Vect3(T x_, T y_, T z_) :x(x_), y(y_), z(z_), w(1) {}




};

typedef Vect3<float> Vect3f; 
typedef Vect3<int> Vect3i;

void splitStr(std::string& str, char delim, std::vector<std::string>& splitString);

template <typename T>
class Buffer
{
public:
	int width;
	int height;
	int size;
	T* data = nullptr;

	Buffer(): width(0), height(0), size(0)
	{}

	Buffer(int w_, int h_) : width(w_), height(h_), size(w_*h_)
	{
		data = new T[size];
		nulling();
	}

	void nulling()
	{
		for (int i = 0; i < size; i++)
		{
			data[i] = 0;
		}
	}

	T& operator()(int j, int i)
	{
		return data[i + j * width];
	}
};


class Matrix4
{
public:

	float& operator()(int j, int i)
	{
		return data[i + j * 4];
	}

private:
	float data[16];

public:
	static Matrix4 set()
	{
		return Matrix4();
	}
};