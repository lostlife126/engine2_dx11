#pragma once
#include "basicStructs.h"

class Camera
{
public:
	float fov;
	float aspect;
	float near;
	float far;

	Matrix4 view;
	Vect3f pos;
	Vect3f up;
	Vect3f front;

};