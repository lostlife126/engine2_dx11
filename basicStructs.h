#pragma once

#define _USE_MATH_DEFINES

#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>

namespace MyEngine
{
	// общие статические структуры 

	double sqr(double x);

	double myMin(double a, double b);

	double myMax(double a, double b);

	float getRand();

	// вычисление угла между x и y через тангенс (от -pi до +pi)
	double getAngle(double x, double y);

	// разделение строки с интами на инты
	void splitStr(std::string& str, char delim, std::vector<int>& splitString);

}

