#include "scene.h"


void Scene::update()
{
	return;
}

void Scene::init()
{
	camera = new Camera;
	allObjects.push_back(new Object);
	return;
}

void Scene::load()
{
	return;
}

void Scene::frustrumCulling()
{
	for (int i = 0; i < allObjects.size(); i++)
	{
		visibleObjects.push(allObjects[i]);
	}
	return;
}

std::queue<Object*>* Scene::getVisibleObjects()
{
	return &visibleObjects;
}

Scene::Scene()
{}

Scene::~Scene()
{}
