#pragma once

#include "log.h"
#include "window.h"
#include "inputManager.h"
#include "scene.h"
#include "renderer.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <omp.h>
#include "GUI.h"

namespace MyEngine
{
	// класс таймера для расчета фпс и дельты по времени
	class FPSCounter
	{
	public:
		// конструктор по умолчанию
		FPSCounter()
		{
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			nulling();
		}
		// зануляем все поля
		void nulling()
		{
			dt = 0.0;
			timeTotal = 0.0;
			QueryPerformanceCounter((LARGE_INTEGER*)&lastFrame);
		}
		// замерить текущий фрейм и дать прошедшее время
		float frame()
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			dt = float(currentTime - lastFrame) / freq;
			lastFrame = currentTime;
			timeTotal += dt; // и считаем текущее общее время
			return dt;
		}

	private:
		INT64 lastFrame; // время последнего замера
		double dt; // дельта времени с последнего замера (время кадра)
		double timeTotal; // общее время прошедшее со времени старта
		INT64 freq; // частота циклов
		INT64 currentTime;
	};

	// класс движка. главный здесь везде. именно в нем содержатся рендерер, менеджер управления, окна и прочие штуки
	class Engine : public InputListener // класс наследник наблюдателя чтобы получать команды от клавы (на выход)
	{
	public:
		// конструктор и деструктор по умолчанию
		Engine();
		~Engine();
		// запускаем ход движка
		void run();
		// инициализируем движок
		void init();
		// вырубаем движок
		void shutdown();
		// нажата ли кнопка (нам будет нужна только кнопка esc)
		bool KeyPressed(const KeyEvent& arg) override;

	protected:

		InputManager* inputManager; // менеджер управления 
		Scene* scene;   // сцена для моделирования 3д
		Renderer* renderer; // отрисовщик
		Window* m_window; // класс окна
		DescWindow desc; // описание окна (зачем оно тут?)
		FPSCounter fpsCounter; // таймер и счетчик фпс
		double dt; // дельта времени с последнего шага
		bool isRun = false; // запущен ли главный цикл движка?
		bool isInitialized = false; // инициализирован ли движок
		Log log; // логер
		// отрисовать сцену
		void drawScene();

		void update();

		float timeDay = 6.0f; // время суток
		bool isStopped = false;

		GUIManager* gui;

	};
}