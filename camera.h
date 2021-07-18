#pragma once
//#include "basicStructs.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
namespace MyEngine
{

	class CameraDX11
	{
	public:

		XMMATRIX m_View;
		XMMATRIX m_Projection;

		XMFLOAT4 pos;
		XMFLOAT4 dir;
		XMFLOAT4 up;
		float aspect;
		float ax;
		float ay;
		float aView;
		bool invertY;

		CameraDX11()
		{
			XMVECTOR pos_ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR at_ = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR up_ = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			pos = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			dir = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
			up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			ax = 0.0f;
			ay = 1.5f;

			invertY = false;
			aspect = 1.33333f;
			aView = 0.6f;
			m_View = XMMatrixLookAtLH(pos_, at_, up_);
			m_Projection = XMMatrixPerspectiveFovLH(aView, aspect, 0.1f, 1000.0f);

		}

		void zoom(float mul)
		{
			aView *= mul;
			if (aView > 1.6f)
			{
				aView = 1.6f;
				return;
			}
			if (aView < 0.5f)
			{
				aView = 0.5f;
				return;
			}
		}

		void render(float dt)
		{
			XMVECTOR pos_ = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
			XMVECTOR at_ = XMVectorSet(pos.x+dir.x, pos.y+dir.y, pos.z+dir.z, 0.0f);
			XMVECTOR up_ = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

			m_View = XMMatrixLookAtLH(pos_, at_, up_);
			m_Projection = XMMatrixPerspectiveFovLH(aView, aspect, 0.1f, 1000.0f);
		}

		void turn(float dx, float dy)
		{
			if (invertY)
			{
				dy = -dy;
			}

			ax += dx;
			ay += dy;
			if (ax > 3.1415926f)
				ax -= 6.2831853f;
			if (ax < -3.1415926f)
				ax += 6.2831853f;
			if (ay < 0.000001f)
				ay = 0.000001f;
			if (ay > 3.1415926f)
				ay = 3.1415926f;

			dir.x = -sinf(ay) * sinf(ax);
			dir.y = cosf(ay);
			dir.z = -sinf(ay) * cosf(ax);
		}

		void move(float dr, float ds)
		{
			pos.x += dir.x * dr + dir.z * ds;
			pos.y += dir.y * dr;
			pos.z += dir.z * dr - dir.x * ds;
		}

		void shift()
		{

		}

	};

}