#pragma once

#include <list>

#include "inputDefines.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//#include "scene.h"
//#include "renderer.h"

namespace MyEngine
{
	// ��� ����������� ���������
	class InputListener;
	// �������� ���������� ���������� ������ ����������� � ���������� ������������ � ��������
	class InputManager
	{
	public:
		// �������������
		void init();
		// ��������
		void close();
		// ��������� ��������
		void run(const UINT& msg, WPARAM wParam, LPARAM lParam);
		// ��������� ���������
		void addListener(InputListener* Listener);
		// ���������� ����???
		void addListeners();
		// ���� ����
		void setWinRect(const RECT& winrect);
		// �������� ���������
		InputListener* getListener();
		// ������������� �������� ���� (���� ������ ������)
		void changeLock();

	private:
		// ������� �������� ����
		void m_eventcursor();
		// ������� ������ ����
		void m_eventmouse(const eMouseKeyCodes KeyCode, bool press);
		// ������� �������� ��������
		void m_mousewheel(short Value);
		// ��������� ������� �������
		void m_eventkey(const eKeyCodes KeyCode, const wchar_t ch, bool press);
		
		std::list<InputListener*> m_Listener; // ������ ����������

		RECT m_windowrect; // ����
		int m_curx; // ���������� ���� x
		int m_cury; // y
		int m_MouseWheel; // ����� ������
		bool lockMouse; // ��������� �� ����
	};

	// ��� ����� ����������� ���������
	class InputListener
	{
	public:
		// ���� ������ ��������� true - ������� ������ ����� �� ��������������

		// ������ ������
		bool MousePressed(const MouseEventClick& arg);
		// ������ ��������
		bool MouseReleased(const MouseEventClick& arg);
		// �������� ��������
		virtual bool MouseWheel(const MouseEventWheel& arg);
		// �������� ����
		virtual bool MouseMove(const MouseEventMove& arg, bool lockedMouse = false);
		// ������ ������
		virtual bool KeyPressed(const KeyEvent& arg);
		// ������ ��������
		bool KeyReleased(const KeyEvent& arg);
		// �������� ����������
		InputManager* manager;
	};

	//------------------------------------------------------------------





}