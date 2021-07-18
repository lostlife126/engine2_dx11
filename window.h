#pragma once


#include <clocale>
#include <ctime>

#include <string>
#include <list>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include"inputManager.h"
//class InputManager;

namespace MyEngine
{

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

		int posx;
		int posy;
		std::wstring caption;	///< заголовок окна
		int width;				///< ширина клиентской части окна
		int height;				///< высота клиентской части окна
		bool resizing;
	};

	class Window
	{
	public:
		Window();

		static Window* get() { return m_wndthis; }

		// Создать окно
		bool create(const DescWindow& desc);

		// Обработка событий окна
		void runEvent();

		// Закрыть окно.
		void close();

		void setInputManager(InputManager* input_manager);

		HWND getHWND() const { return m_hwnd; }
		int getLeft() const { return m_desc.posx; }
		int getTop() const { return m_desc.posy; }
		int getWidth() const { return m_desc.width; }
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
		InputManager* inputManager;
		HWND m_hwnd;		// дескриптор окна	
		bool m_isexit;		// флаг сообщающий о событии выхода	
		bool m_active;		// окно активно?
		bool m_minimized;
		bool m_maximized;
		bool m_isresize;	// если окно изменило размер
	};

	// обработка событий
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

}