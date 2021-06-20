#pragma once
#include "basicStructs.h"

class Camera
{
public:
	float fov;
	float aspect;
	float near;
	float far;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	Vect3f pos;
	Vect3f up;
	Vect3f front;

	void setMatrices()
	{
		viewMatrix = Matrix4::setView(pos, up, front);
		projectionMatrix = Matrix4::setProjection(fov, aspect, near, far);
	}

};