#pragma once

#define _USE_MATH_DEFINES

#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>

namespace MyEngine
{
	/*
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

		Vect3() :x(0), y(0), z(0), w(1) {}
		Vect3(T v) :x(v), y(v), z(v), w(1) {}
		Vect3(T x_, T y_, T z_) :x(x_), y(y_), z(z_), w(1) {}

		Vect3 operator*(T a)
		{
			return Vect3(x * a, y * a, z * a);
		}

		T dot(const Vect3& v)
		{
			return x * v.x + y * v.y + z * v.z;
		}

		Vect3<T> cross(const Vect3& v)
		{
			return Vect3
			(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			);
		}

		T len2()
		{
			return x * x + y * y + z * z;
		}

		T len()
		{
			return sqrt(x * x + y * y + z * z);
		}

		T& normalize()
		{
			float r = 1.0f / len();
			x *= r;
			y *= r;
			z *= r;
			return *this;
		}

		Vect3<T> reflect(const Vect3<T>& n)
		{

			return Vect3<T>();
		}

	};

	typedef Vect3<float> Vect3f;
	typedef Vect3<int> Vect3i;
	*/

	void splitStr(std::string& str, char delim, std::vector<int>& splitString);


	/*
	float edge(Vect3f& a, Vect3f& b, Vect3f& c);

	float min(float a, float b);
	float max(float a, float b);
	float min(float a, float b, float c);
	float max(float a, float b, float c);

	int min(int a, int b);
	int max(int a, int b);
	int min(int a, int b, int c);
	int max(int a, int b, int c);




	class Matrix4
	{
	public:

		Matrix4()
		{
			for (int i = 0; i < 16; i++)
			{
				data[i] = 0.0;
			}
		}

		float& operator()(int j, int i)
		{
			return data[i + j * 4];
		}

		const float& operator()(int j, int i) const
		{
			return data[i + j * 4];
		}

	private:
		float data[16];

	public:
		static Matrix4 setProjection(float fov, float aspect, float nearPoint, float farPoint)
		{
			Matrix4 mat;
			float f = 1.0f / tan(fov * 0.5f * M_PI / 180.0f);
			mat(0, 0) = f;
			mat(1, 1) = f * aspect;
			mat(2, 2) = -nearPoint / (farPoint - nearPoint);
			mat(2, 3) = farPoint * nearPoint / (farPoint - nearPoint);
			mat(3, 2) = 1.0f;
			return mat;
		}

		static Matrix4 setOne()
		{
			Matrix4 mat;
			mat(0, 0) = 1.0f;
			mat(1, 1) = 1.0f;
			mat(2, 2) = 1.0f;
			mat(3, 3) = 1.0f;
			return mat;
		}

		static Matrix4 setView(Vect3f& pos, Vect3f& up, Vect3f front)
		{
			Matrix4 mat;
			Vect3f right = up.cross(front);

			mat(0, 0) = right.x;
			mat(0, 1) = right.y;
			mat(0, 2) = right.z;
			mat(0, 3) = -pos.dot(right);

			mat(1, 0) = up.x;
			mat(1, 1) = up.y;
			mat(1, 2) = up.z;
			mat(1, 3) = -pos.dot(up);

			mat(2, 0) = front.x;
			mat(2, 1) = front.y;
			mat(2, 2) = front.z;
			mat(2, 3) = -pos.dot(front);

			mat(3, 3) = 1.0;

			return mat;
		}

		Vect3f mul(const Vect3f& v)
		{
			Vect3f res;
			res.x = v.x * (*this)(0, 0) + v.y * (*this)(0, 1) + v.z * (*this)(0, 2) + (*this)(0, 3);
			res.y = v.x * (*this)(1, 0) + v.y * (*this)(1, 1) + v.z * (*this)(1, 2) + (*this)(1, 3);
			res.z = v.x * (*this)(2, 0) + v.y * (*this)(2, 1) + v.z * (*this)(2, 2) + (*this)(2, 3);
			res.w = v.x * (*this)(3, 0) + v.y * (*this)(3, 1) + v.z * (*this)(3, 2) + (*this)(3, 3);
			return res;
		}

		Matrix4 operator*(const Matrix4& m)
		{
			Matrix4 res;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					res(i, j) =
						+(*this)(i, 0) * m(0, j)
						+ (*this)(i, 1) * m(1, j)
						+ (*this)(i, 2) * m(2, j)
						+ (*this)(i, 3) * m(3, j);
				}
			return res;
		}
	};*/
}

