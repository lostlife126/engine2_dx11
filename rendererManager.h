#pragma once
#include "sceneManager.h"

class RendererManager
{
public:
	void render();
	void init();

private:
	SceneManager* sceneManager;
	
};