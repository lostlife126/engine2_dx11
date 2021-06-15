#pragma once
#include "basicStructs.h"
#include "mesh.h"

class Object
{
public:

	Object() 
	{
		mesh = new Mesh;
		mesh->init();
	}

private:
	Mesh* mesh = nullptr;
	Matrix4 mat;
};

class Light
{

};