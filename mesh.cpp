#include "mesh.h"

void Mesh::load(const std::string& path)
{
	loadObj(path);
}

void Mesh::loadObj(const std::string& path)
{
	std::ifstream file;
	file.open(path.c_str());
	if (!file.good())
	{
		std::cout << "Error loading file: "<<path.c_str() << std::endl;
		return;
	}

	std::string line, key, x, y, z;
	float tempU, tempV, intpart;
	Vect3i indices[3];
	char delim = '/';
	while (!file.eof())
	{
		std::getline(file, line);
		std::istringstream iss(line);
		iss >> key;
		if (key == "v") // vertex
		{
			iss >> x >> y >> z;
			Vect3f v(std::stof(x), std::stof(y), std::stof(z));
			vertices.push_back(v);
			continue;
		}
		if (key == "vn") // normals
		{
			iss >> x >> y >> z;
			Vect3f vn(std::stof(x), std::stof(y), std::stof(z));
			normals.push_back(vn);
			continue;
		}
		if (key == "vt") // texture
		{
			iss >> x >> y;
			Vect3f vt(std::stof(x), std::stof(y), 0);
			texels.push_back(vt);
			continue;
		}
		if (key == "f") // indices of face
		{
			iss >> x >> y >> z;
			std::vector<std::string> splitX; splitStr(x, delim, splitX);
			std::vector<std::string> splitY; splitStr(y, delim, splitY);
			std::vector<std::string> splitZ; splitStr(z, delim, splitZ);
			for (int i = 0; i < splitX.size(); i++)
			{
				indices[i] = Vect3i(std::stoi(splitX[i]) - 1, std::stoi(splitY[i]) - 1, std::stoi(splitZ[i]) - 1);
			}

			vertexIndices.push_back(indices[0]);
			textureIndices.push_back(indices[1]);
			normalsIndices.push_back(indices[2]);
			continue;
		}
		if (key.c_str()[0] == '#') // comment
		{
			continue;
		}
		if (key == "o") // caption
		{
			iss >> caption;
			continue;
		}
		if (key == "mtllib")
		{
			continue;
		}
	}
	numVertices = vertices.size();
	numFaces = vertexIndices.size();
	return;
}