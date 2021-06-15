#pragma once

#include "world.h"
#include "sceneManager.h"
#include "rendererManager.h"



class Engine
{
public:

	World world;
	SceneManager sceneManager;
	RendererManager rendererManager;

	bool isRun = false;
	bool isInitialized = false;

	void run();
	void start();
	void init();
};