#include "model.h"

namespace MyEngine
{

	void loadObjInfo(const char* path, int& nVertices, int& nTexels, int& nNormals, int& nFaces)
	{
		nVertices = 0;
		nTexels = 0;
		nNormals = 0;
		nFaces = 0;
		std::ifstream file;
		file.open(path);
		if (!file.good())
		{
			Log::Get()->Error("Error opening file: ", path);
			return;
		}
		std::string line, key;
		while (!file.eof())
		{
			std::getline(file, line);
			std::istringstream iss(line);
			key = "";
			iss >> key;
			if (key == "v") // vertex
			{
				nVertices++;
				continue;
			}
			if (key == "vn") // vertex
			{
				nNormals++;
				continue;
			}
			if (key == "vt") // vertex
			{
				nTexels++;
				continue;
			}
			if (key == "f") // vertex
			{
				nFaces++;
				continue;
			}
		}
		file.close();
		return;
	}

}