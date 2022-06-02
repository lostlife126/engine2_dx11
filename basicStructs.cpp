#include "basicStructs.h"

namespace MyEngine
{

	double sqr(double x)
	{
		return x * x;
	}

	double myMin(double a, double b)
	{
		return (a > b)?b:a;
	}

	double myMax(double a, double b)
	{
		return (a > b) ? a : b;
	}

	float getRand()
	{
		return (rand() % 32768) / 32768.0;
	}

	double getAngle(double x, double y)
	{
		double res;
		if (fabs(x) > 1e-10)
			res = atanf(y / fabs(x));
		else
			res = M_PI_2;
		if (x < 0.0) // угол в левой половине (2 3 четверти)
			res = M_PI - res;
		return res;
	}

	void splitStr(std::string& str, char delim, std::vector<int>& splitString)
	{
		std::stringstream ss(str);
		std::string token;
		while (std::getline(ss, token, delim))
		{
			if (token == "")
				splitString.push_back(0);
			else
				splitString.push_back(std::stoi(token));
		}
		return;
	}
}