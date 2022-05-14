#include "inputManager.h"
#include"log.h"

namespace MyEngine
{
	//------------------------------------------------------------------

	void InputManager::init()
	{
		m_MouseWheel = m_curx = m_cury = 0;
		lockMouse = false;
		Log::Get()->Debug("InputManager init");
	}

	void InputManager::close()
	{
		if (!m_Listener.empty())
			m_Listener.clear();
		Log::Get()->Debug("InputManager close");
	}

	void InputManager::setWinRect(const RECT& winrect)
	{
		m_windowrect.left = winrect.left;
		m_windowrect.right = winrect.right;
		m_windowrect.top = winrect.top;
		m_windowrect.bottom = winrect.bottom;
	}

	InputListener* InputManager::getListener()
	{
		return m_Listener.front();
	}

	void InputManager::addListener(InputListener* Listener)
	{
		m_Listener.push_back(Listener);
		m_Listener.back()->manager = this;
	}

	void InputManager::addListeners()
	{
		m_Listener.push_back(new InputListener);
		m_Listener.back()->manager = this;
	}

	void InputManager::changeLock()
	{
		POINT Position;
		GetCursorPos(&Position);	// получаем текущую позицию курсора

		Position.x -= m_windowrect.left;
		Position.y -= m_windowrect.top;
		if (Position.x > 0 && Position.x < 640 && Position.y>0 && Position.y < 480)
		{
			lockMouse = !lockMouse;
			ShowCursor(!lockMouse);
		}
		return;
	}

	void InputManager::run(const UINT& msg, WPARAM wParam, LPARAM lParam)
	{
		if (m_Listener.empty())
			return;

		eKeyCodes KeyIndex;
		wchar_t buffer[1];
		BYTE lpKeyState[256];

		m_eventcursor();// событие движения мыши
		switch (msg)
		{
		case WM_KEYDOWN:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			m_eventkey(KeyIndex, buffer[0], true);
			break;
		case WM_KEYUP:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			m_eventkey(KeyIndex, buffer[0], false);
			break;
		case WM_LBUTTONDOWN:
			m_eventmouse(MOUSE_LEFT, true);
			break;
		case WM_LBUTTONUP:
			m_eventmouse(MOUSE_LEFT, false);
			break;
		case WM_RBUTTONDOWN:
			m_eventmouse(MOUSE_RIGHT, true);
			break;
		case WM_RBUTTONUP:
			m_eventmouse(MOUSE_RIGHT, false);
			break;
		case WM_MBUTTONDOWN:
			m_eventmouse(MOUSE_MIDDLE, true);
			break;
		case WM_MBUTTONUP:
			m_eventmouse(MOUSE_MIDDLE, false);
			break;
		case WM_MOUSEWHEEL:
			m_mousewheel((short)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;
		}
	}

	void InputManager::m_eventcursor()
	{
		POINT Position;
		GetCursorPos(&Position);	// получаем текущую позицию курсора

		Position.x -= m_windowrect.left;
		Position.y -= m_windowrect.top;

		if (m_curx == Position.x && m_cury == Position.y)
			return;

		int dx = Position.x - m_curx;
		int dy = Position.y - m_cury;
		if (!lockMouse)
		{
			m_curx = Position.x;
			m_cury = Position.y;
		}
		else
		{
			SetCursorPos(m_curx + m_windowrect.left, m_cury + m_windowrect.top);
		}

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseMove(MouseEventMove(m_curx, m_cury, dx, dy), lockMouse) == true)
				return;
		}
	}

	void InputManager::m_eventmouse(const eMouseKeyCodes Code, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			// кнопка нажата
			if (press == true)
			{
				if ((*it)->MousePressed(MouseEventClick(Code, m_curx, m_cury)) == true)
					return;
			}
			// кнопка отпущена
			else
			{
				if ((*it)->MouseReleased(MouseEventClick(Code, m_curx, m_cury)) == true)
					return;
			}
		}
	}

	void InputManager::m_mousewheel(short Value)
	{
		m_MouseWheel = Value;

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseWheel(MouseEventWheel(m_MouseWheel, m_curx, m_cury)) == true)
				return;
		}
	}

	void InputManager::m_eventkey(const eKeyCodes KeyCode, const wchar_t ch, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			// кнопка нажата
			if (press == true)
			{
				if ((*it)->KeyPressed(KeyEvent(ch, KeyCode)) == true)
					return;
			}
			// кнопка отпущена
			else
			{
				if ((*it)->KeyReleased(KeyEvent(ch, KeyCode)) == true)
					return;
			}
		}
	}


	bool InputListener::MousePressed(const MouseEventClick& arg) { return false; }
	// кнопка отпущена
	bool InputListener::MouseReleased(const MouseEventClick& arg) { return false; }
	// вращение колесика
	bool InputListener::MouseWheel(const MouseEventWheel& arg) { return false; }
	// движение мыши
	bool InputListener::MouseMove(const MouseEventMove& arg, bool lockedMouse){	return false; }
	// кнопка нажата
	bool InputListener::KeyPressed(const KeyEvent& arg) { return false;	}
	// кнопка отпущена
	bool InputListener::KeyReleased(const KeyEvent& arg) { return false; }

}