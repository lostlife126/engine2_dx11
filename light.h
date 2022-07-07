#pragma once

#include "basicStructs.h"
namespace MyEngine
{
	// ����� ������� ������ �������� �����
	class Light
	{
	public:

		int type; /// 0 - ������������, 1 - ��������, 2 - ���������????
		float a; // ������� ���� ��������� ��������� �����
		XMFLOAT3 pos;// ������� ���������� ���������
		XMFLOAT3 dir; // ����������� �����
		XMFLOAT4 color; // ������������ �����
		XMMATRIX m_View;// ������� �������
		// ����������� �� ��������� ������� ���� � ����� 1,0,0 � ��������� � 0,0,0
		Light(int type = 0, float a = 0.0)
		{
			pos = XMFLOAT3(cosf(a), 0.0f, sinf(a));
			dir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		void resetViewMatrix()
		{
			XMVECTOR pos_ = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
			XMVECTOR at_ = XMVectorSet(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z, 0.0f);
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			m_View = XMMatrixLookAtLH(pos_, at_, up);
		}
		// ������� ���� � ����� xyz
		Light(int type, XMFLOAT3 pos_, XMFLOAT3 dir_):
			pos(pos_),
			dir(dir_)
		{
			pos = XMFLOAT3(cosf(a), 0.0, sinf(a));
			dir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		XMMATRIX getViewMatrix()
		{
			return m_View;
		}
		// �������� ��������� � ����������� �����
		void update(float timeDay)
		{
			a = (timeDay - 6.0) / 12.0 * M_PI;
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
			redFrac *= 0.5;
			otherFrac *= 0.5;

			color = XMFLOAT4(redFrac, otherFrac, otherFrac, 1.0f);

			pos = XMFLOAT3(50.0 * cosf(a), 50.0 * sinf(a), 0.0);
			dir = XMFLOAT3(-cosf(a), -sinf(a), 0.0f);
		}
	};
	//
}