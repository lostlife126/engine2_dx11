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

		return false;
	}

	void Engine::drawScene()
	{
		renderer->driverDX11->setRenderTargetBuffers(); // ������������� ���� ���������� ���� ������
		renderer->driverDX11->clearRenderTarget();  // ������ ������

		scene->drawAll(); // ������ ��� ������� ������� ����

		renderer->driverDX11->setRenderTargetBackBuffer(); // ������������� ���� ���������� ������ �����

		renderer->driverDX11->beginScene(); // ������ ������

		renderer->driverDX11->turnZBufferOff(); // ��������� z 

		renderer->driverDX11->renderToScreen(); // ������������� ������� � ������� � ������

		renderer->driverDX11->renderShader(scene->getLight(), scene->getCamera()); // �������� ���� ������ �� ������ ����� ��������� ����

		gui->render(renderer->driverDX11, dt, timeDay, scene->getCamera()->getPosition(), scene->getCamera()->getDirection());

		renderer->driverDX11->turnZBufferOn();

		renderer->driverDX11->endScene();

		timeDay += dt;
		if (timeDay > 24.0f)
		{
			timeDay -= 24.0f;
		}

	}
}