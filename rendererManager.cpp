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
	renderer->zBuff->rescale();
	ImageBMP image(*(renderer->zBuff));
	image.save("1.bmp");

	return;
}

void RendererManager::init(SceneManager* sm)
{
	sceneManager = sm;
	renderer = new Renderer;
	renderer->camera = sm->getScene()->getCamera();
	renderer->zBuff = new Buffer<float>(1024, 768);
	return;
}