#include "sceneManager.h"
namespace MyEngine
{
	SceneManager::SceneManager() :
		scene(nullptr)
	{}

	SceneManager::~SceneManager()
	{
		close();
	}

	void SceneManager::update(float dt)
	{
		scene->update(dt);
		return;
	}

	void SceneManager::init(ID3D11Device* deviceContext)
	{
		scene = new Scene;
		scene->init(deviceContext);
		return;
	}

	Scene* SceneManager::getScene()
	{
		return scene;
	}

	void SceneManager::close()
	{
		delete scene;
	}
}