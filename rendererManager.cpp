#include "rendererManager.h"

void RendererManager::renderScene()
{
	Scene* scene = sceneManager->getScene();
	std::queue<Object*>* obj = scene->getVisibleObjects();
	renderer->zBuff->nulling();

	while (!obj->empty())
	{
		renderer->drawObject(obj->front());
		obj->pop();
	}
	

	for (int i = 0; i < renderer->zBuff->size; i++)
	{
		renderer->zBuff->data[i] = gammaTable[int(renderer->zBuff->data[i]*255.0f)];
	}

	renderer->zBuff->saveBMP("zBuff.bmp");
	renderer->pixBuff->saveBMP("screen.bmp");

	return;
}

void RendererManager::init(SceneManager* sm)
{
	sceneManager = sm;
	renderer = new Renderer;
	renderer->camera = sm->getScene()->getCamera();
	renderer->zBuff = new Buffer<float>(width, height);
	renderer->pixBuff = new Buffer<uint32_t>(width, height);
	return;
}