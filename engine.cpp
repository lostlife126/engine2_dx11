#include "engine.h"
#include <stdlib.h>

namespace MyEngine
{

	void Engine::run()
	{
		isRun = true;
		fpsCounter.nulling();
		dt = 0.0;
		while (isRun)
		{
			scene->update(dt, timeDay);
			drawScene();
			m_window->runEvent();
			dt = fpsCounter.frame();
			if (dt < 0.01)
			{
				Sleep(int(1000.0 * (0.01 - dt)));
				dt = 0.01;
			}
		}
		return;
	}

	void Engine::shutdown()
	{
		delete scene;
		delete renderer;
		delete inputManager;
		delete m_window;
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

		inputManager->addListener(scene->getCamera());
		inputManager->addListener(this);

		isInitialized = true;

		gui = new GUIManager(renderer->driverDX11->getDevice(), renderer->driverDX11->getDeviceContext());
		gui->init();
	}

	Engine::~Engine()
	{
		shutdown();
	}

	Engine::Engine()
	{

	}

	bool Engine::KeyPressed(const KeyEvent& arg)
	{
		printf("key press %c\n", arg.wc);
			if (arg.code == VK_ESCAPE)
				isRun = false;
			if (arg.wc == 'z')
				isStopped = false;
			if (arg.wc == 'x')
				isStopped = true;

		return false;
	}

	void Engine::drawScene()
	{

		renderer->driverDX11->clearRenderTarget();  // чистим буферы

		//renderer->driverDX11->setRenderTargetBackBuffer(); // устанавливаем цель рендеринга задний буфер

		//renderer->driverDX11->setRenderTargetShadows(); // устанавливаем цель рендеринга наши буферы
		//scene->drawAllShadows();
		//renderer->driverDX11->turnZBufferOff(); // выключаем z 
		//gui->render(renderer->driverDX11, dt, timeDay, scene->getCamera()->getPosition(), scene->getCamera()->getDirection());
		//renderer->driverDX11->turnZBufferOn();

	//	renderer->driverDX11->endScene();
		


		// делаем рендеринг сцены во все нужные буферы
		renderer->driverDX11->setRenderTargetBuffers(); // устанавливаем цель рендеринга наши буферы

		scene->drawAllOpaqueCam(); // рисуем все непрозрачные объекты которые надо

		//////////////////////
		//// здесь будем рендерить все прозрачные материалы?

		scene->drawAllTransparent();

		/// <summary>
		/// теперь рисуем из буферов в задний буфер
		/// </summary>

		renderer->driverDX11->setRenderTargetBackBuffer(); // устанавливаем цель рендеринга задний буфер
		renderer->driverDX11->beginScene(); // чистим буферы
		renderer->driverDX11->turnZBufferOff(); // выключаем z 
		renderer->driverDX11->renderToScreen(); // устанавливаем вершины и индексы в шейдер
		renderer->driverDX11->renderShader(scene->getLight(), scene->getCamera()); // рендерим наши буферы на задний буфер использу€ свет
		/// <summary>
		/// рендерим GUI
		/// </summary>
		gui->render(renderer->driverDX11, dt, timeDay, scene->getCamera()->getPosition(), scene->getCamera()->getDirection());
		renderer->driverDX11->turnZBufferOn();

		renderer->driverDX11->endScene();

		if (!isStopped)
		{
			timeDay += dt;
		}
		if (timeDay > 24.0f)
		{
			timeDay -= 24.0f;
		}

		

	}
}