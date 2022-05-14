#pragma once

#include "log.h"
#include "window.h"
#include "inputManager.h"
#include "scene.h"
#include "renderer.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <omp.h>

namespace MyEngine
{
	// ����� ������� ��� ������� ��� � ������ �� �������
	class FPSCounter
	{
	public:
		// ����������� �� ���������
		FPSCounter()
		{
			nulling();
		}
		// �������� ��� ����
		void nulling()
		{
			dt = 0.0;
			timeTotal = 0.0;
			lastFrame = omp_get_wtime();
		}
		// �������� ������� ����� � ���� ��������� �����
		float frame()
		{
			double timeNow = omp_get_wtime(); // ���������� ���������� omp.h
			dt = (timeNow - lastFrame);
			lastFrame = timeNow;
			timeTotal += dt; // � ������� ������� ����� �����
			return dt;
		}

	private:
		double lastFrame; // ����� ���������� ������
		double dt; // ������ ������� � ���������� ������ (����� �����)
		double timeTotal; // ����� ����� ��������� �� ������� ������
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
		double timeFPS; // ����� ��������� � ���������� ��������� ������ � ��� (��� ���������� ��� � 0.25 ���)
		bool isRun = false; // ������� �� ������� ���� ������?
		bool isInitialized = false; // ��������������� �� ������
		Log log; // �����
		// ���������� �����
		void drawScene();

	};
}