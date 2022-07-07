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

		renderer->driverDX11->clearRenderTarget();  // ������ ������

		//renderer->driverDX11->setRenderTargetBackBuffer(); // ������������� ���� ���������� ������ �����

		//renderer->driverDX11->setRenderTargetShadows(); // ������������� ���� ���������� ���� ������
		//scene->drawAllShadows();
		//renderer->driverDX11->turnZBufferOff(); // ��������� z 
		//gui->render(renderer->driverDX11, dt, timeDay, scene->getCamera()->getPosition(), scene->getCamera()->getDirection());
		//renderer->driverDX11->turnZBufferOn();

	//	renderer->driverDX11->endScene();
		


		// ������ ��������� ����� �� ��� ������ ������
		renderer->driverDX11->setRenderTargetBuffers(); // ������������� ���� ���������� ���� ������

		scene->drawAllOpaqueCam(); // ������ ��� ������������ ������� ������� ����

		//////////////////////
		//// ����� ����� ��������� ��� ���������� ���������?

		scene->drawAllTransparent();

		/// <summary>
		/// ������ ������ �� ������� � ������ �����
		/// </summary>

		renderer->driverDX11->setRenderTargetBackBuffer(); // ������������� ���� ���������� ������ �����
		renderer->driverDX11->beginScene(); // ������ ������
		renderer->driverDX11->turnZBufferOff(); // ��������� z 
		renderer->driverDX11->renderToScreen(); // ������������� ������� � ������� � ������
		renderer->driverDX11->renderShader(scene->getLight(), scene->getCamera()); // �������� ���� ������ �� ������ ����� ��������� ����
		/// <summary>
		/// �������� GUI
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