#pragma once

#define _USE_MATH_DEFINES

#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>

namespace MyEngine
{
	// ����� ����������� ��������� 

	double sqr(double x);

	double myMin(double a, double b);

	double myMax(double a, double b);

	float getRand();

	// ���������� ���� ����� x � y ����� ������� (�� -pi �� +pi)
	double getAngle(double x, double y);

	// ���������� ������ � ������ �� ����
	void splitStr(std::string& str, char delim, std::vector<int>& splitString);

}

