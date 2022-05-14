#pragma once
#include <queue>
#include "scene.h"
#include "videoDriverDX11.h"
#include "shader.h"
#include "font.h"
#include "string.h"
#include "sstream"

namespace MyEngine
{// тут ничего интересного
	class Renderer
	{
	public:
		void renderScene(float dt, double time);
		void init(HWND hwnd_);
		VideoDriverDX11* driverDX11;

		//Font font;
	//	Text* text_fps;
	//	Text* text_time;

	private:
		
		int m_width = 1024;
		int m_height = 768;
		HWND hwnd;

	};
}