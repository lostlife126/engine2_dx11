#pragma once

#include <list>

#include "inputDefines.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//#include "scene.h"
//#include "renderer.h"

namespace MyEngine
{
	// наш наблюдатель слушатель
	class InputListener;
	// менеджер управления использует шаблон наблюдателя и уведомляет наблюдателей о событиях
	class InputManager
	{
	public:
		// инициализация
		void init();
		// закрытие
		void close();
		// прогоняем менеджер
		void run(const UINT& msg, WPARAM wParam, LPARAM lParam);
		// добавляем слушателя
		void addListener(InputListener* Listener);
		// добалвляем себя???
		void addListeners();
		// зона окна
		void setWinRect(const RECT& winrect);
		// получить слушателя
		InputListener* getListener();
		// заблокировать движение мыши (можт убрать отсюда)
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
		
		std::list<InputListener*> m_Listener; // список слушателей

		RECT m_windowrect; // окно
		int m_curx; // координаты мыши x
		int m_cury; // y
		int m_MouseWheel; // сдвиг колеса
		bool lockMouse; // заблочена ли мышь
	};

	// сам класс наблюдатель слушатель
	class InputListener
	{
	public:
		// если методы возращают true - событие больше никем не обрабатывается

		// кнопка нажата
		bool MousePressed(const MouseEventClick& arg);
		// кнопка отпущена
		bool MouseReleased(const MouseEventClick& arg);
		// вращение колесика
		virtual bool MouseWheel(const MouseEventWheel& arg);
		// движение мыши
		virtual bool MouseMove(const MouseEventMove& arg, bool lockedMouse = false);
		// кнопка нажата
		virtual bool KeyPressed(const KeyEvent& arg);
		// кнопка отпущена
		bool KeyReleased(const KeyEvent& arg);
		// менеджер управления
		InputManager* manager;
	};

	//------------------------------------------------------------------





}