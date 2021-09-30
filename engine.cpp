#include "engine.h"

namespace MyEngine
{

	void Engine::run()
	{
		fpsCounter.nulling();
		while (isRun)
		{
			float dt = fpsCounter.frame();
			scene->update(dt);
			drawScene();
			m_window->runEvent();

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
		m_window = new Window;
		m_window->create(desc);

		inputManager = new InputManager();
		inputManager->init();
		inputManager->addListeners();
		m_window->setInputManager(inputManager);

		renderer = new Renderer;
		renderer->init(m_window->getHWND());

		scene = new Scene;
		scene->init(renderer->driverDX11);

		inputManager->getListener()->scene = scene;
		inputManager->getListener()->renderer = renderer;

		isRun = false;
		isInitialized = true;
	}

	Engine::~Engine()
	{
		delete scene;
		delete renderer;
	}

	Engine::Engine()
	{

	}

	void Engine::drawScene()
	{
		renderer->driverDX11->setRenderTargets();
		renderer->driverDX11->clearRenderTargets();

		scene->drawAll();

		renderer->driverDX11->beginScene();
		
		renderer->driverDX11->turnZBufferOff();


		renderer->driverDX11->renderOrtho();
		renderer->driverDX11->turnZBufferOn();

		//turnOffZ();
		// full screen window render
		// light shader render
		// turnOnZ()

		renderer->driverDX11->endScene();


	}
}