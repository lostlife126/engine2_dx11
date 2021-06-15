#include "engine.h"

void Engine::run()
{
	while (isRun)
	{
		world.update();
		sceneManager.update();
		rendererManager.render();

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
	world.init();
	sceneManager.init();
	rendererManager.init();
	isRun = false;
	isInitialized = true;
}