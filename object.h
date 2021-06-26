#pragma once
#include "basicStructs.h"
#include"buffer.h"
#include "mesh.h"

class Object
{
public:

	Object() 
	{
		mesh = new Mesh;
		mesh->load("firehydrant_mesh.obj");
		mat = Matrix4::setOne();

		texture = new Texture;
		texture->loadBMP("firehydrant_albedo.bmp");
	}

	Mesh* getMesh()
	{
		return mesh;
	}

	Texture* getTexture()
	{
		return texture;
	}

	Matrix4* getMatrix()
	{
		return &mat;
	}

private:
	Mesh* mesh = nullptr;
	Matrix4 mat;
	Texture* texture;
};

class Light
{

};