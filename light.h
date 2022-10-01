#pragma once

#include "basicStructs.h"
namespace MyEngine
{
	// класс который хранит источник света
	class Light
	{
	public:

		int m_type; /// 0 - направленный, 1 - точечный, 2 - прожектор????
		XMFLOAT3 m_pos;// текущая координата источника
		XMFLOAT3 m_dir; // направление света
		XMFLOAT4 m_color; // цвет света
		XMMATRIX m_View;// видовая матрица
		// конструктор по умолчанию создает свет в точке 1,0,0 и направлен в 0,0,0
		Light():
			m_type(0),
			m_pos(0.0f, 0.0f, 0.0f),
			m_dir(1.0f, 0.0f, 0.0f),
			m_color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			updateViewMatrix();
		}

		void updateViewMatrix()
		{
			XMVECTOR pos = XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 1.0f);
			XMVECTOR at = XMVectorSet(m_pos.x + m_dir.x, m_pos.y + m_dir.y, m_pos.z + m_dir.z, 0.0f);
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			m_View = XMMatrixLookAtLH(pos, at, up);
		}
		// создать свет в точке xyz
		Light(int type, XMFLOAT3 pos, XMFLOAT3 dir, XMFLOAT4 color):
			m_type(type),
			m_pos(pos),
			m_dir(dir),
			m_color(color)
		{
			updateViewMatrix();
		}

		XMMATRIX getViewMatrix()
		{
			return m_View;
		}
	};

	class Sun : public Light
	{
	public:

		void update(float timeDay)
		{
			float a = (timeDay - 6.0) / 12.0 * M_PI;
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

			m_color = XMFLOAT4(redFrac, otherFrac, otherFrac, 1.0f);

			m_pos = XMFLOAT3(50.0 * cosf(a), 50.0 * sinf(a), 0.0);
			m_dir = XMFLOAT3(-cosf(a), -sinf(a), 0.0f);
			updateViewMatrix();
		}
	};
	//
}