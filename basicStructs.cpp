#include "basicStructs.h"

void splitStr(std::string& str, char delim, std::vector<std::string>& splitString)
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim))
	{
		if (token == "")
			splitString.push_back("0");
		else
			splitString.push_back(token);
	}
	return;
}

float edge(Vect3f& a, Vect3f& b, Vect3f& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	//return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

float min(float a, float b)
{
	return (a < b) ? a : b;
}

float max(float a, float b)
{
	return (a > b) ? a : b;
}

float min(float a, float b, float c)
{
	return min(min(a, b), c);
}

float max(float a, float b, float c)
{
	return max(max(a, b), c);
}

int min(int a, int b)
{
	return (a < b) ? a : b;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int min(int a, int b, int c)
{
	return min(min(a, b), c);
}

int max(int a, int b, int c)
{
	return max(max(a, b), c);
}