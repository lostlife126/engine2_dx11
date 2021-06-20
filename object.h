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
		mat = Matrix4::setOne();
	}

	Mesh* getMesh()
	{
		return mesh;
	}

	Matrix4* getMatrix()
	{
		return &mat;
	}

private:
	Mesh* mesh = nullptr;
	Matrix4 mat;
};

class Light
{

};