#include "basicStructs.h"

void splitStr(std::string& str, char delim, std::vector<std::string>& splitString)
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim))
	{
		if (token == "")
		{
			splitString.push_back("0");
		}
		else
		{
			splitString.push_back(token);
		}
	}
	return;
}