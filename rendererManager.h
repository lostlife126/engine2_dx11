#pragma once
#include <queue>
#include "sceneManager.h"
#include "renderer.h"

class RendererManager
{
public:
	void renderScene();
	void init(SceneManager* sm);

private:
	SceneManager* sceneManager;
	Renderer* renderer;

	int width = 1024;
	int height = 768;
};