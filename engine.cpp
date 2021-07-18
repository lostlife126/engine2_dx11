#include "engine.h"

namespace MyEngine
{

	void Engine::run()
	{
		while (isRun)
		{
			sceneManager->update();
			rendererManager->renderScene();
			m_window->runEvent();

		//	isRun = true;
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
		if (!m_window->create(desc))
		{
			Log::Get()->Error("Can't create window.");
			return;
		}

		inputManager = new InputManager();
		inputManager->init();
		inputManager->addListeners();
		m_window->setInputManager(inputManager);
		rendererManager = new Renderer;
		rendererManager->init(sceneManager, m_window->getHWND());
		sceneManager = new SceneManager;
		sceneManager->init(rendererManager->driverDX11->m_pDevice);
		rendererManager->sceneManager = sceneManager;
		inputManager->getListener()->scene = sceneManager->getScene();
		inputManager->getListener()->renderer = rendererManager;

		isRun = false;
		isInitialized = true;
	}

	Engine::~Engine()
	{
		delete sceneManager;
		delete rendererManager;
	}

	Engine::Engine()
	{

	}
}