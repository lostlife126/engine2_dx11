#pragma once

#include <clocale>
#include <string>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include"inputManager.h"

namespace MyEngine
{
	// структура свойств окна
	struct DescWindow
	{
		DescWindow() :
			caption(L""),
			width(640),
			height(480),
			posx(200),
			posy(20),
			resizing(true)
		{}

		int posx; // положение х верхнего левого края
		int posy; // положение у верхнего левого края
		std::wstring caption;	//< заголовок окна
		int width;				//< ширина клиентской части окна
		int height;				//< высота клиентской части окна
		bool resizing; // 
	};

	// класс инкапсулирующий окно в windows
	class Window
	{
	public:
		// конструктор по умолчанию
		Window();

		// получение окна (в одном экземпляре)
		static Window* get() { return m_wndthis; }

		// Создать окно
		bool create(const DescWindow& desc);

		// Обработка событий окна
		void runEvent();

		// Закрыть окно.
		void close();
		// задать менеджер управления
		void setInputManager(InputManager* input_manager);
		// получить hwnd
		HWND getHWND() const { return m_hwnd; }
		// получить левый край
		int getLeft() const { return m_desc.posx; }
		// получить верхний край
		int getTop() const { return m_desc.posy; }
		// получить ширину
		int getWidth() const { return m_desc.width; }
		// получить высоту
		int getHeight() const { return m_desc.height; }
		// Вернуть заголовок окна
		const std::wstring& getCaption() const { return m_desc.caption; }

		// сообщает, было ли сообщение о выходе
		bool isExit() const { return m_isexit; }
		// сообщает об активности окна
		bool isActive() const { return m_active; }
		// сообщает об изменении окна
		// предупреждение: после вызова оповещает окно об обработке события
		bool isResize()
		{
			bool ret = m_isresize;
			m_isresize = false;
			return ret;
		}

		// обработка событий
		LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	private:
		void m_UpdateWindowState();

		static Window* m_wndthis;

		DescWindow m_desc;	// описание окна
		InputManager* inputManager; // менеджер управления
		HWND m_hwnd;		// дескриптор окна	
		bool m_isexit;		// флаг сообщающий о событии выхода	
		bool m_active;		// окно активно?
		bool m_minimized; // свернуто
		bool m_maximized; // развернуто
		bool m_isresize;	// если окно изменило размер
	};

	// обработка событий WndProc
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

}