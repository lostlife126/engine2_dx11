#include "engine.h"
#include <stdlib.h>

namespace MyEngine
{

	void Engine::run()
	{
		isRun = true;
		fpsCounter.nulling();
		while (isRun)
		{
			dt = fpsCounter.frame();
			scene->update(dt);
			drawScene();
			m_window->runEvent();
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

		return false;
	}

	void Engine::drawScene()
	{
		renderer->driverDX11->setRenderTargetBuffers(); // устанавливаем цель рендеринга наши буферы
		renderer->driverDX11->clearRenderTarget();  // чистим буферы

		scene->drawAll(); // рисуем все объекты которые надо

		renderer->driverDX11->setRenderTargetBackBuffer(); // устанавливаем цель рендеринга задний буфер

		renderer->driverDX11->beginScene(); // чистим буферы

		renderer->driverDX11->turnZBufferOff(); // выключаем z 

		renderer->driverDX11->renderToScreen(); // устанавливаем вершины и индексы в шейдер

		renderer->driverDX11->renderShader(scene->getLight(), scene->getCamera()); // рендерим наши буферы на задний буфер использу€ свет

		if (timeFPS > 0.25)
		{
			char bufferFPS[12];
			char bufferNum[5];
			strcpy_s(bufferFPS, "fps = ");
			_itoa_s( int(1.0 / dt), bufferNum, 10);
			strcat_s(bufferFPS, bufferNum);
			renderer->driverDX11->textFPS->setText(bufferFPS);
			timeFPS = 0.0;
			int hours = timeDay;
			int minutes = (timeDay - hours) * 60.0f;
			strcpy_s(bufferFPS, "time: ");
			_itoa_s(hours, bufferNum, 10);
			if(hours < 10)
				strcat_s(bufferFPS, "0");
			strcat_s(bufferFPS, bufferNum);
			strcat_s(bufferFPS, ":");
			if (minutes < 10)
				strcat_s(bufferFPS, "0");
			_itoa_s(minutes, bufferNum, 10);
			strcat_s(bufferFPS, bufferNum);
			renderer->driverDX11->textTimeDay->setText(bufferFPS);
		}
		XMFLOAT4 colorFPS(0.0, 1.0, 0.0, 1.0);
		if (1.0 / dt < 300.0)
		{
			colorFPS = XMFLOAT4(1.0, 0.0, 0.0, 1.0);
		}
		renderer->driverDX11->renderText(renderer->driverDX11->textFPS, colorFPS, XMFLOAT2(0, 0));
		renderer->driverDX11->renderText(renderer->driverDX11->textControl, XMFLOAT4(1.0, 1.0, 1.0, 1.0), XMFLOAT2(0, 450));
		renderer->driverDX11->renderText(renderer->driverDX11->textTimeDay, XMFLOAT4(1.0, 1.0, 1.0, 1.0), XMFLOAT2(0, 40));
		renderer->driverDX11->turnZBufferOn();

		renderer->driverDX11->endScene();

		timeFPS += dt;
		timeDay += dt;
		if (timeDay > 24.0f)
		{
			timeDay -= 24.0f;
		}

	}
}