#pragma once

#include "basicStructs.h"
namespace MyEngine
{
	// класс который хранит источник света
	class Light
	{
	public:

		float a; // текущий угол положения источника света
		XMFLOAT3 pos;// текущая координата источника
		XMFLOAT3 lightDir; // направление света
		XMFLOAT4 lightAmbient; // составляющая среды
		XMFLOAT4 lightDiffuse; // диффузная составляющая
		XMFLOAT4 lightSpect; // диффузная составляющая
		// конструктор по умолчанию создает свет в точке 1,0,0 и направлен в 0,0,0
		Light()
		{
			a = 0.0;
			pos = XMFLOAT3(cosf(a), 0.0f, sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			lightAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
			lightDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			lightSpect = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		// создать свет в точке xyz
		Light(float x, float y, float z)
		{
			a = getAngle(x, z);
			pos = XMFLOAT3(cosf(a), y, sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			lightAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
			lightDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			lightSpect = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		// обновить положение и направление света
		void update(float dt)
		{
			a += 0.1 * dt;
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

			lightDiffuse = XMFLOAT4(redFrac, otherFrac, otherFrac, 1.0f);

			pos = XMFLOAT3(50.0 * cosf(a), 50.0 * sinf(a), 0.0);
			lightDir = XMFLOAT3(-cosf(a), -sinf(a), 0.0f);
		}
	};

}