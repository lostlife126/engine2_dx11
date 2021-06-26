#pragma once
#include "basicStructs.h"
#include "buffer.h"
#include "object.h"
#include "camera.h"


class Renderer
{
public:

	Buffer<float>* zBuff;
	Buffer<uint32_t>* pixBuff;
	Camera* camera;
	

	void drawObject(Object* obj);
	void rasterize(Vect3f* v, Vect3f* uv, Texture* texture);

};