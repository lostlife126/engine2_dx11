#pragma once

#include "font.h"
#include <stdlib.h>

namespace MyEngine
{

	class GUIManager
	{
	public:

		ID3D11Device* device;
		ID3D11DeviceContext* context;

		Font* fontFPS;
		Font* fontControl;
		float timeReload;

		Text* textFPS; // текст: fps = ...
		Text* textControl; // текст подсказки по управлению
		Text* textTimeDay; // текст подсказки по управлению
		Text* textPos; // текст подсказки по управлению
		Text* textDir; // текст подсказки по управлению

		GUIManager(ID3D11Device* device_, ID3D11DeviceContext* context_):
			device(device_),
			context(context_)
		{}

		void init()
		{
			int m_widthScreen = 640;
			int m_heightScreen = 480;
			fontFPS = new Font(device, context);
			fontFPS->init(m_widthScreen, m_heightScreen, 10);
			textFPS = new Text(fontFPS);
			textFPS->init("fps = ", 12, true);
			fontControl = new Font(device, context);
			fontControl->init(m_widthScreen, m_heightScreen, 7);
			textControl = new Text(fontControl);
			textControl->init("w - forward, s - back, a - left, d - right, q - lock mouse, esc - exit", 72, false);
			textTimeDay = new Text(fontControl);
			textTimeDay->init("time: ", 12, true);
			textPos = new Text(fontControl);
			textPos->init("pos: ", 40, true);
			textDir = new Text(fontControl);
			textDir->init("dir: ", 40, true);
		}

		void resetFPS(float dt)
		{
			char bufferFPS[12];
			char bufferNum[5];
			strcpy_s(bufferFPS, "fps = ");
			_itoa_s(int(1.0 / dt), bufferNum, 10);
			strcat_s(bufferFPS, bufferNum);
			textFPS->setText(bufferFPS);
		}

		void resetPos(XMFLOAT3 pos)
		{
			char bufferFPS[40];
			char bufferNum[5];
			strcpy_s(bufferFPS, "pos: ");
			std::string my_val = std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z);
			strcat_s(bufferFPS, my_val.c_str());
			textPos->setText(bufferFPS);
		}

		void resetDir(XMFLOAT3 dir)
		{
			char bufferFPS[40];
			char bufferNum[5];
			strcpy_s(bufferFPS, "dir: ");
			std::string my_val = std::to_string(dir.x) + ", " + std::to_string(dir.y) + ", " + std::to_string(dir.z);
			strcat_s(bufferFPS, my_val.c_str());
			textDir->setText(bufferFPS);
		}

		void resetDayTime(float dayTime)
		{
			char bufferDayTime[12];
			char bufferNum[5];
			int hours = dayTime;
			int minutes = (dayTime - hours) * 60.0f;
			strcpy_s(bufferDayTime, "time: ");
			_itoa_s(hours, bufferNum, 10);
			if (hours < 10)
				strcat_s(bufferDayTime, "0");
			strcat_s(bufferDayTime, bufferNum);
			strcat_s(bufferDayTime, ":");
			if (minutes < 10)
				strcat_s(bufferDayTime, "0");
			_itoa_s(minutes, bufferNum, 10);
			strcat_s(bufferDayTime, bufferNum);
			textTimeDay->setText(bufferDayTime);
		}

		void render(VideoDriverDX11* driver, float dt, float dayTime, XMFLOAT3 pos, XMFLOAT3 dir)
		{
			if (timeReload > 0.25)
			{
				resetFPS(dt);
				resetDayTime(dayTime);
				resetPos(pos);
				resetDir(dir);
				timeReload = 0.0;
			}
			XMFLOAT4 colorFPS(0.0, 1.0, 0.0, 1.0);
			if (1.0 / dt < 300.0)
			{
				colorFPS = XMFLOAT4(1.0, 0.0, 0.0, 1.0);
			}
			driver->renderText(textFPS, colorFPS, XMFLOAT2(0, 0));
			driver->renderText(textControl, XMFLOAT4(1.0, 1.0, 1.0, 1.0), XMFLOAT2(0, 450));
			driver->renderText(textTimeDay, XMFLOAT4(1.0, 1.0, 1.0, 1.0), XMFLOAT2(0, 40));
			driver->renderText(textPos, XMFLOAT4(1.0, 1.0, 1.0, 1.0), XMFLOAT2(0, 80));
			driver->renderText(textDir, XMFLOAT4(1.0, 1.0, 1.0, 1.0), XMFLOAT2(0, 120));
			timeReload += dt;
		}
	};

}