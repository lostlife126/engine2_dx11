#include "scene.h"
namespace MyEngine
{

	CameraDX11* Scene::getCamera()
	{
		return m_camera;
	}

	void Scene::update(float dt)
	{
		
		m_camera->render(dt);
		frustrumCulling();
		light[0]->update(dt);
		return;
	}

	void Scene::init(VideoDriverDX11* driver_)
	{
		driver = driver_;
		m_camera = new CameraDX11;
		addObject("chest_mesh.obj", "chest_albedo2.png");
		light.push_back(new Light);
		return;
	}

	void Scene::addObject(const char* pathMesh, const char* pathTexture)
	{
		allObjects.push_back(new Model);
		allObjects.back()->load(driver->getDevice(), pathMesh, pathTexture);
	}

	void Scene::drawAll()
	{
		for (auto obj : allObjects)
		{
			obj->render(driver->getDeviceContext(), obj->m_World, m_camera->m_View, m_camera->m_Projection);
		}
	}

	void Scene::frustrumCulling()
	{
		for (int i = 0; i < allObjects.size(); i++)
		{
		//	visibleObjects.push(allObjects[i]);
		}
		return;
	}


	Scene::Scene()
	{}

	Scene::~Scene()
	{}
}