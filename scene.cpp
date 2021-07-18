#include "scene.h"
namespace MyEngine
{

	void Scene::update()
	{
		frustrumCulling();
		return;
	}

	void Scene::init(ID3D11Device* deviceContext)
	{
		/*
		camera = new CameraSoft;
		camera->fov = 70.0f;
		camera->aspect = 1.33333f;
		camera->nearPoint = 0.1f;
		camera->farPoint = 100.0f;
		camera->pos = Vect3f(0.0f, 0.0f, 2.0f);
		camera->up = Vect3f(0.0f, 1.0f, 0.0f);
		camera->front = Vect3f(0.0f, 0.0f, -1.0f);
		
		camera->setMatrices();*/
		allObjects.push_back(new Model);
		allObjects[0]->load(deviceContext, "chest_mesh.obj", "chest_albedo2.png");
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

	std::queue<Model*>* Scene::getVisibleObjects()
	{
		return &visibleObjects;
	}

	Scene::Scene()
	{}

	Scene::~Scene()
	{}
}