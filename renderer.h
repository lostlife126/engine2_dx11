#pragma once
#include "basicStructs.h"
#include "object.h"
#include "camera.h"

class Renderer
{
public:

	Buffer<float>* zBuff;
	Camera* camera;
	

	void drawObject(Object* obj);
	void rasterize(Vect3f* v);

};