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
		XMFLOAT3 direction; // ����������� �����
		XMFLOAT4 ambientColor; // ������������ �����
		XMFLOAT4 diffuseColor; // ��������� ������������
		XMFLOAT4 specularColor; // ��������� ������������
		float specularPower = 100.0;
		// ����������� �� ��������� ������� ���� � ����� 1,0,0 � ��������� � 0,0,0
		Light()
		{
			a = 0.0;
			pos = XMFLOAT3(cosf(a), 0.0f, sinf(a));
			direction = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			ambientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
			diffuseColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// ������� ���� � ����� xyz
		Light(float x, float y, float z)
		{
			a = getAngle(x, z);
			pos = XMFLOAT3(cosf(a), y, sinf(a));
			direction = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			ambientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
			diffuseColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// �������� ��������� � ����������� �����
		void update(float dt)
		{
			a += M_PI / 12.0 * dt;
			float redFrac = 1.0f;
			float otherFrac = 1.0f;	
			if (a > M_PI * 2.0)
				a -= M_PI * 2.0;
			float horizontHei = myMin(a, M_PI - a);
			if (horizontHei < 0.0)
			{
				redFrac = 0.0f;
				otherFrac = 0.0f;
			}
			else
			{
				if (horizontHei < M_PI / 6.0)
				{
					redFrac = horizontHei / (M_PI / 6.0);
					otherFrac = 0.0;
				}
				else
				{
					if (horizontHei < M_PI / 3.0)
					{
						redFrac = 1.0f;
						otherFrac = (horizontHei - (M_PI / 6.0)) / (M_PI / 6.0);
					}
					else
					{
						redFrac = 1.0f;
						otherFrac = 1.0f;
					}
				}
			}

			diffuseColor = XMFLOAT4(redFrac, otherFrac, otherFrac, 1.0f);

			pos = XMFLOAT3(50.0 * cosf(a), 50.0 * sinf(a), 0.0);
			direction = XMFLOAT3(-cosf(a), -sinf(a), 0.0f);
		}
	};

}