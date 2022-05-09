#pragma once

#include <d3d11.h>
//#include <d3dx11.h>
//#include <d3dcompiler.h>
#include <xnamath.h>
//#include<d3dx10math.h>

class Light
{
public:

	float a;
	XMFLOAT3 pos;
	XMFLOAT3 lightDir;
	XMFLOAT4 lightAmbient;
	XMFLOAT4 lightDiffuse;

	Light()
	{
		a = 0.0;
		pos = XMFLOAT3(cosf(a), 0.0f, sinf(a));
		lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
		lightAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
		lightDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	void update(float dt)
	{
		a += dt;
		pos = XMFLOAT3(5.0*cosf(a), 0.0f, 5.0 * sinf(a));
		lightDir = XMFLOAT3(-cosf(a), 0.0f, -sinf(a));
	}
};