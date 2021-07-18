#pragma once

#include <list>

#include "inputDefines.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "scene.h"
#include "renderer.h"

namespace MyEngine
{
	class InputListener;

	class InputManager
	{
	public:
		void init();
		void close();

		void run(const UINT& msg, WPARAM wParam, LPARAM lParam);

		void addListener(InputListener* Listener);

		void addListeners();

		// зона окна
		void setWinRect(const RECT& winrect);

		InputListener* getListener();

		void changeLock();

	private:
		// событие движения мыши
		void m_eventcursor();
		// событие кнопки мыши
		void m_eventmouse(const eMouseKeyCodes KeyCode, bool press);
		// событие вращения колесика
		void m_mousewheel(short Value);
		// обработка события клавиши
		void m_eventkey(const eKeyCodes KeyCode, const wchar_t ch, bool press);

		std::list<InputListener*> m_Listener;

		RECT m_windowrect;
		int m_curx;
		int m_cury;
		int m_MouseWheel;
		bool lockMouse;
	};


	class InputListener
	{
	public:
		// если методы возращают true - событие больше никем не обрабатывается

		// кнопка нажата
		bool MousePressed(const MouseEventClick& arg) { return false; }
		// кнопка отпущена
		bool MouseReleased(const MouseEventClick& arg) { return false; }
		// вращение колесика
		bool MouseWheel(const MouseEventWheel& arg) 
		{ 
			if(arg.wheel > 0)
				scene->camera.zoom(1.1f);
			else
				scene->camera.zoom(0.91f);
			return false; 
		}
		// движение мыши
		bool MouseMove(const MouseEventMove& arg, bool lockedMouse = false) 
		{ 
			if (lockedMouse)
			{
				scene->camera.turn(arg.dx * 0.001f, arg.dy * 0.001f);
			}
			return false; 
		}

		// кнопка нажата
		bool KeyPressed(const KeyEvent& arg) 
		{
			printf("key press %c\n", arg.wc); 
			if (arg.wc == 'w')
				scene->camera.move(0.04f, 0.0f);
			if (arg.wc == 's')
				scene->camera.move(-0.04f, 0.0f);
			if (arg.wc == 'a')
				scene->camera.move(0.0f, -0.04f);
			if (arg.wc == 'd')
				scene->camera.move(0.0f, 0.04f);
			if (arg.wc == 'q')
				manager->changeLock();
			if (arg.wc == 'e')
				renderer->driverDX11->changeState();
				

			return false;
		}
		// кнопка отпущена
		bool KeyReleased(const KeyEvent& arg) { return false; }


		Scene* scene;
		Renderer* renderer;
		InputManager* manager;
	};

	//------------------------------------------------------------------





}