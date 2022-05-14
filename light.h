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
		// конструктор по умолчанию создает свет в точке 1,0,0 и направлен в 0,0,0
		Light()
		{
			a = 0.0;
			pos = XMFLOAT3(cosf(a), 0.0f, sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			lightAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
			lightDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		// создать свет в точке xyz
		Light(float x, float y, float z)
		{
			a = getAngle(x, z);
			pos = XMFLOAT3(cosf(a), y, sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
			lightAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
			lightDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		// обновить положение и направление света
		void update(float dt)
		{
			a += dt;
			pos = XMFLOAT3(5.0 * cosf(a), pos.y, 5.0 * sinf(a));
			lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
		}
	};

}