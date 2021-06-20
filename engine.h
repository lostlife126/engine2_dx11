#pragma once

#include "world.h"
#include "sceneManager.h"
#include "rendererManager.h"


class Engine
{
public:

	Engine();
	~Engine();

	World* world;
	SceneManager* sceneManager;
	RendererManager* rendererManager;

	bool isRun = false;
	bool isInitialized = false;

	void run();
	void start();
	void init();
};