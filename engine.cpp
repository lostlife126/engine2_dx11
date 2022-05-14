#include "engine.h"

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
		renderer->driverDX11->setRenderTargetBuffers(); // ������������� ���� ���������� ���� ������
		renderer->driverDX11->clearRenderTarget();  // ������ ������

		scene->drawAll(); // ������ ��� ������� ������� ����

		renderer->driverDX11->setRenderTargetBackBuffer(); // ������������� ���� ���������� ������ �����

		renderer->driverDX11->beginScene(); // ������ ������

		renderer->driverDX11->turnZBufferOff(); // ��������� z 

		renderer->driverDX11->renderToScreen(); // ������������� ������� � ������� � ������

		renderer->driverDX11->renderShader(scene->getLight()); // �������� ���� ������ �� ������ ����� ��������� ����

		if (timeFPS > 0.25)
		{
			std::stringstream ss;
			ss << int(1.0 / dt);
			std::string str;
			ss >> str;
			std::string str2 = "fps = ";
			str2 += str; // ������������� ������� fps = 
			renderer->driverDX11->textFPS->setText(str2.c_str());	
			timeFPS = 0.0;
		}
		XMFLOAT4 colorFPS(0.0, 1.0, 0.0, 1.0);
		if (1.0 / dt < 300.0)
		{
			colorFPS = XMFLOAT4(1.0, 0.0, 0.0, 1.0);
		}
		renderer->driverDX11->renderText(renderer->driverDX11->textFPS, colorFPS, XMFLOAT2(0, 0));
		renderer->driverDX11->renderText(renderer->driverDX11->textControl, XMFLOAT4(1.0, 1.0, 1.0, 1.0), XMFLOAT2(0, 450));
		renderer->driverDX11->turnZBufferOn();

		renderer->driverDX11->endScene();

		timeFPS += dt;

	}
}