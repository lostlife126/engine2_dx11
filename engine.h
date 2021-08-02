#pragma once

#include "log.h"
#include "window.h"
#include "inputManager.h"
#include "sceneManager.h"
#include "renderer.h"
#include <d3d11.h>
#include <d3dx11.h>
#include "font.h"
#include <omp.h>

namespace MyEngine
{

	class FPSCounter
	{
	public:

		FPSCounter()
		{
			nulling();
		}

		void nulling()
		{
			dt = 0.0;
			time = 0.0;
			lastFrame = omp_get_wtime();
		}

		float frame()
		{
			double timeNow = omp_get_wtime();
			dt = (timeNow - lastFrame);
			lastFrame = timeNow;
			time += dt;
			return dt;
		}
		double lastFrame;
		double dt;
		double time;
	private:

	};

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
		FPSCounter fpsCounter;


		bool isRun = false;
		bool isInitialized = false;
		Log log;

		void run();
		void start();
		void init();
	};
}