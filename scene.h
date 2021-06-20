#pragma once

#include <vector>
#include <queue>
#include "camera.h"
#include "object.h"



class Scene
{
public:

	Scene();
	~Scene();

	void update();
	void init();

	std::queue<Object*>* getVisibleObjects();

	Camera* getCamera()
	{
		return camera;
	}

private:
	void load();

	std::vector<Object*> allObjects;
	std::queue<Object*> visibleObjects;
	Camera* camera = nullptr;

	void frustrumCulling();
};




