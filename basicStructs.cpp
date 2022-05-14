#include "basicStructs.h"

namespace MyEngine
{

	double getAngle(double x, double y)
	{
		double res;
		if (fabs(x) > 1e-10)
			res = atanf(y / fabs(x));
		else
			res = M_PI_2;
		if (x < 0.0) // ���� � ����� �������� (2 3 ��������)
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