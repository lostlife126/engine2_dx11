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
	// ����� ������� ��� ������� ��� � ������ �� �������
	class FPSCounter
	{
	public:
		// ����������� �� ���������
		FPSCounter()
		{
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			nulling();
		}
		// �������� ��� ����
		void nulling()
		{
			dt = 0.0;
			timeTotal = 0.0;
			QueryPerformanceCounter((LARGE_INTEGER*)&lastFrame);
		}
		// �������� ������� ����� � ���� ��������� �����
		float frame()
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			dt = float(currentTime - lastFrame) / freq;
			lastFrame = currentTime;
			timeTotal += dt; // � ������� ������� ����� �����
			return dt;
		}

	private:
		INT64 lastFrame; // ����� ���������� ������
		double dt; // ������ ������� � ���������� ������ (����� �����)
		double timeTotal; // ����� ����� ��������� �� ������� ������
		INT64 freq; // ������� ������
		INT64 currentTime;
	};

	// ����� ������. ������� ����� �����. ������ � ��� ���������� ��������, �������� ����������, ���� � ������ �����
	class Engine : public InputListener // ����� ��������� ����������� ����� �������� ������� �� ����� (�� �����)
	{
	public:
		// ����������� � ���������� �� ���������
		Engine();
		~Engine();
		// ��������� ��� ������
		void run();
		// �������������� ������
		void init();
		// �������� ������
		void shutdown();
		// ������ �� ������ (��� ����� ����� ������ ������ esc)
		bool KeyPressed(const KeyEvent& arg) override;

	protected:

		InputManager* inputManager; // �������� ���������� 
		Scene* scene;   // ����� ��� ������������� 3�
		Renderer* renderer; // ����������
		Window* m_window; // ����� ����
		DescWindow desc; // �������� ���� (����� ��� ���?)
		FPSCounter fpsCounter; // ������ � ������� ���
		double dt; // ������ ������� � ���������� ����
		bool isRun = false; // ������� �� ������� ���� ������?
		bool isInitialized = false; // ��������������� �� ������
		Log log; // �����
		// ���������� �����
		void drawScene();

		void update();

		float timeDay = 6.0f; // ����� �����
		bool isStopped = false;

		GUIManager* gui;

	};
}