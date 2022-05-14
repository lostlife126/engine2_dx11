#pragma once

#include "basicStructs.h"
namespace MyEngine
{
	// ����� ������� ������ �������� �����
	class Light
	{
	public:

		float a; // ������� ���� ��������� ��������� �����
		XMFLOAT3 pos;// ������� ���������� ���������
		XMFLOAT3 lightDir; // ����������� �����
		XMFLOAT4 lightAmbient; // ������������ �����
		XMFLOAT4 lightDiffuse; // ��������� ������������
		// ����������� �� ��������� ������� ���� � ����� 1,0,0 � ��������� � 0,0,0
		Light()
		{
			a = 0.0;
			pos = XMFLOAT3(cosf(a), 0.0f, sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			lightAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
			lightDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		// ������� ���� � ����� xyz
		Light(float x, float y, float z)
		{
			a = getAngle(x, z);
			pos = XMFLOAT3(cosf(a), y, sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			lightAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
			lightDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		// �������� ��������� � ����������� �����
		void update(float dt)
		{
			a += dt;
			pos = XMFLOAT3(5.0 * cosf(a), pos.y, 5.0 * sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
		}
	};

}