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
		std::wstring caption;	///< ��������� ����
		int width;				///< ������ ���������� ����� ����
		int height;				///< ������ ���������� ����� ����
		bool resizing;
	};

	class Window
	{
	public:
		Window();

		static Window* get() { return m_wndthis; }

		// ������� ����
		bool create(const DescWindow& desc);

		// ��������� ������� ����
		void runEvent();

		// ������� ����.
		void close();

		void setInputManager(InputManager* input_manager);

		HWND getHWND() const { return m_hwnd; }
		int getLeft() const { return m_desc.posx; }
		int getTop() const { return m_desc.posy; }
		int getWidth() const { return m_desc.width; }
		int getHeight() const { return m_desc.height; }
		// ������� ��������� ����
		const std::wstring& getCaption() const { return m_desc.caption; }

		// ��������, ���� �� ��������� � ������
		bool isExit() const { return m_isexit; }
		// �������� �� ���������� ����
		bool isActive() const { return m_active; }
		// �������� �� ��������� ����
		// ��������������: ����� ������ ��������� ���� �� ��������� �������
		bool isResize()
		{
			bool ret = m_isresize;
			m_isresize = false;
			return ret;
		}

		// ��������� �������
		LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	private:
		void m_UpdateWindowState();

		static Window* m_wndthis;

		DescWindow m_desc;	// �������� ����
		InputManager* inputManager;
		HWND m_hwnd;		// ���������� ����	
		bool m_isexit;		// ���� ���������� � ������� ������	
		bool m_active;		// ���� �������?
		bool m_minimized;
		bool m_maximized;
		bool m_isresize;	// ���� ���� �������� ������
	};

	// ��������� �������
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

}