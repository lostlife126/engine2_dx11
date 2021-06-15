#include "sceneManager.h"

SceneManager::SceneManager(): 
	scene(nullptr) 
{}

SceneManager::~SceneManager()
{
	close();
}

void SceneManager::update()
{
	scene->update();
	return;
}

void SceneManager::init()
{
	scene = new Scene;
	scene->init();
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