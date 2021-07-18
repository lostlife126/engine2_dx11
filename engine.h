#pragma once

#include "log.h"
#include "window.h"
#include "inputManager.h"
#include "sceneManager.h"
#include "renderer.h"
#include <d3d11.h>
#include <d3dx11.h>

namespace MyEngine
{

	class Engine
	{
	public:

		Engine();
		~Engine();

		InputManager* inputManager;
		SceneManager* sceneManager;
		Renderer* rendererManager;
		Window* m_window;
		DescWindow desc;

		bool isRun = false;
		bool isInitialized = false;
		Log log;

		void run();
		void start();
		void init();
	};
}