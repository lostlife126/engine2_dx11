#pragma once
//#include "basicStructs.h"
#include <d3d11.h>
//#include <d3dx11.h>

#include <xnamath.h>
#include "inputManager.h"

namespace MyEngine
{

	const float maxSpeed = 50.0;
	const float acceleration = 5.0;

	// ����� ������. ������������� ���������� �������

	class Camera : public InputListener
	{
	public:
		// ����������� �� ��������� (� ��������� 0,0,0, � ����������� �� z )
		Camera() :
			position(0.0, 0.0, 0.0, 0.0),
			speed(0.0, 0.0, 0.0, 0.0),
			direction(0.0, 0.0, 1.0, 0.0),
			zoomNow(1.0),
			invertY(false)

		{
			setInit();
		}

		void* operator new(size_t i)
		{
			return _aligned_malloc(i, 16);
		}
		// ������� ���� �����������
		void operator delete(void* p)
		{
			_aligned_free(p);
		}
		
		// ���� ������ ������ (����� ���������� wasd)
		bool KeyPressed(const KeyEvent& arg) override;
		// ���� ��������� ���� �� ��������� ������� (���� ���� �������������)
		bool MouseMove(const MouseEventMove& arg, bool lockedMouse) override;
		// ���� ������� ������ �� �������� ��� (� ������� ����)
		bool MouseWheel(const MouseEventWheel& arg) override;
		
		// ��������� ������ � ���������� x y z, � ������������ �� z
		Camera(float x, float y, float z) :
			position(x, y, z, 0.0),
			speed(0.0, 0.0, 0.0, 0.0),
			direction(0.0, 0.0, 1.0, 0.0),
			zoomNow(1.0),
			invertY(false)

		{
			setInit();
		}
		// ���������� �������� ������ (�������� ���������� � ����������� ������� ����)
		void render(float dt);
		// �������� ������� ��������
		XMMATRIX getViewMatrix();
		// �������� ������������ �������
		XMMATRIX getProjectionMatrix();

	private:

		XMMATRIX m_View;// ������� �������
		XMMATRIX m_Projection; // ������������ �������
		XMMATRIX m_baseView; // ��������� ������� ��� ����������� ����������

		XMFLOAT4 position;// ������� ���������
		XMFLOAT4 speed; // ������� ��������
		XMFLOAT4 direction; // ������� �����������

		float aspectRatio; // ��������� ����������� ������
		float ax;// ������� ���� �������� � ��������� xz (��������)
		float ay; // ������� ���� �� ��� � (����)
		float zoomNow; // ������� ���
		bool invertY; // ������������� �������� �� ��� y
		bool moved; // �������� �������� ������?
		// ���� ������� �������� ������
		void moveForward();
		// ���� ������� �������� �����
		void moveBack();
		// ���� ������� �������� �����
		void moveLeft();
		// ���� ������� �������� ������
		void moveRight();
		// ��� �� ��������� mul 
		void zoom(float mul);
		// ������������ �������
		void turn(float dx, float dy);
		// ��������� �������� (�� ������ ������������)
		void correctSpeed();
		// ������������� ������
		void setInit();

	};

}