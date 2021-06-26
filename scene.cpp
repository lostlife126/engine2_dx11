#include "scene.h"


void Scene::update()
{
	frustrumCulling();
	return;
}

void Scene::init()
{
	camera = new Camera;
	camera->fov = 70.0f;
	camera->aspect = 1.33333f;
	camera->near = 0.1f;
	camera->far = 100.0f;
	camera->pos = Vect3f(0.5f, 0.5f, 2.0f);
	camera->up = Vect3f(0.0f, 1.0f, 0.0f);
	camera->front = Vect3f(0.0f, 0.0f, -1.0f);

	camera->setMatrices();
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
