#include "engine.h"

void Engine::run()
{
	while (isRun)
	{
		world->update();
		sceneManager->update();
		rendererManager->renderScene();

		isRun = false;
	}
	return;
}

void Engine::start()
{
	init();
	isRun = true;
	run();
	return;
}
void Engine::init()
{
	world = new World;
	world->init();
	sceneManager = new SceneManager;
	sceneManager->init();
	rendererManager = new RendererManager;
	rendererManager->init(sceneManager);
	isRun = false;
	isInitialized = true;
}

Engine::~Engine()
{
	delete world;
	delete sceneManager;
	delete rendererManager;
}

Engine::Engine()
{

}