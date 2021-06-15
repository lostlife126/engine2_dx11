#pragma once
#include "basicStructs.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Mesh
{
public:

	void init();

	std::string caption;

	std::vector<Vect3f> vertices;
	std::vector<Vect3f> normals;
	std::vector<Vect3f> texels;

	std::vector<Vect3i> vertexIndices;
	std::vector<Vect3i> normalsIndices;
	std::vector<Vect3i> textureIndices;

	int numVertices;
	int numFaces;

private:
	void loadObj(std::string path);
};