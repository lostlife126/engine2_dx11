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
		XMFLOAT4 speed;
		XMFLOAT4 dir;
		float a_speed = 0.5f;
		float aspect;
		float ax;
		float ay;
		float aView;
		bool invertY;

		CameraDX11()
		{
			XMVECTOR pos_ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR at_ = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			pos = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			dir = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
			speed = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			ax = 0.0f;
			ay = 1.5f;

			invertY = false;
			aspect = 1.33333f;
			aView = 0.6f;
			m_View = XMMatrixLookAtLH(pos_, at_, up);
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
			pos.x += speed.x * dt;
			pos.y += speed.y * dt;
			pos.z += speed.z * dt;

			XMVECTOR pos_ = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
			XMVECTOR at_ = XMVectorSet(pos.x+dir.x, pos.y+dir.y, pos.z+dir.z, 0.0f);
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			speed.x *= 0.98; if (fabs(speed.x) < 0.01f) speed.x = 0.0f;
			speed.y *= 0.98; if (fabs(speed.y) < 0.01f) speed.y = 0.0f;
			speed.z *= 0.98; if (fabs(speed.z) < 0.01f) speed.z = 0.0f;

			m_View = XMMatrixLookAtLH(pos_, at_, up);
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

		void correctSpeed()
		{
			if (speed.x < -1.0f) speed.x = -1.0f; if (speed.x > 1.0f) speed.x = 1.0f;
			if (speed.y < -1.0f) speed.y = -1.0f; if (speed.y > 1.0f) speed.y = 1.0f;
			if (speed.z < -1.0f) speed.z = -1.0f; if (speed.z > 1.0f) speed.z = 1.0f;
		}

		void moveForward()
		{
			speed.x += a_speed * dir.x;
			speed.y += a_speed * dir.y;
			speed.z += a_speed * dir.z;
			correctSpeed();
		}

		void moveBack()
		{
			speed.x -= a_speed * dir.x;
			speed.y -= a_speed * dir.y;
			speed.z -= a_speed * dir.z;
			correctSpeed();
		}
		void moveLeft()
		{
			speed.x -= a_speed * dir.z;
			speed.z += a_speed * dir.x;
			correctSpeed();
		}
		void moveRight()
		{
			speed.x += a_speed * dir.z;
			speed.z -= a_speed * dir.x;
			correctSpeed();
		}

	};

}