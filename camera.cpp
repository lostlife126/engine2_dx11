#include "camera.h"

namespace MyEngine
{

	bool Camera::KeyPressed(const KeyEvent& arg)
	{
		printf("key press %c\n", arg.wc);
		if (arg.wc == 'w')
			moveForward();
		if (arg.wc == 's')
			moveBack();
		if (arg.wc == 'a')
			moveLeft();
		if (arg.wc == 'd')
			moveRight();
		if (arg.wc == 'q')
			manager->changeLock();
		return false;
	}

	bool Camera::MouseMove(const MouseEventMove& arg, bool lockedMouse)
	{
		if (lockedMouse)
		{
			turn(arg.dx * 0.001f, arg.dy * 0.001f);
		}
		return false;
	}

	bool Camera::MouseWheel(const MouseEventWheel& arg)
	{
		if (arg.wheel > 0)
			zoom(1.1f);
		else
			zoom(0.91f);
		return false;
	}

	void Camera::moveForward()
	{
		speed.x += acceleration * direction.x;
		speed.y += acceleration * direction.y;
		speed.z += acceleration * direction.z;
		moved = true;
		correctSpeed();
	}

	void Camera::moveBack()
	{
		speed.x -= acceleration * direction.x;
		speed.y -= acceleration * direction.y;
		speed.z -= acceleration * direction.z;
		moved = true;
		correctSpeed();
	}

	void Camera::moveLeft()
	{
		speed.x -= acceleration * direction.z;
		speed.z += acceleration * direction.x;
		moved = true;
		correctSpeed();
	}

	void Camera::moveRight()
	{
		speed.x += acceleration * direction.z;
		speed.z -= acceleration * direction.x;
		moved = true;
		correctSpeed();
	}

	void Camera::zoom(float mul)
	{
		zoomNow *= mul;
		if (zoomNow > 1.6f)
		{
			zoomNow = 1.6f;
		}
		else
		{
			if (zoomNow < 0.5f)
			{
				zoomNow = 0.5f;
			}
		}
		m_Projection = XMMatrixPerspectiveFovLH(zoomNow, aspectRatio, 0.1f, 1000.0f);
		return;
	}

	void Camera::render(float dt)
	{
		if (!moved)
		{
			speed.x *= 0.95; if (fabs(speed.x) < 0.01f) speed.x = 0.0f;
			speed.y *= 0.95; if (fabs(speed.y) < 0.01f) speed.y = 0.0f;
			speed.z *= 0.95; if (fabs(speed.z) < 0.01f) speed.z = 0.0f;
		}
		moved = false;

		position.x += speed.x * dt;
		position.y += speed.y * dt;
		position.z += speed.z * dt;

		XMVECTOR pos_ = XMVectorSet(position.x, position.y, position.z, 0.0f);
		XMVECTOR at_ = XMVectorSet(position.x + direction.x, position.y + direction.y, position.z + direction.z, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_View = XMMatrixLookAtLH(pos_, at_, up);
	}

	void Camera::turn(float dx, float dy)
	{
		if (invertY)
			dy = -dy;

		ax -= dx;
		ay += dy;
		if (ax > XM_PI)
			ax - XM_PI;
		if (ax < -XM_PI)
			ax = XM_PI;
		if (ay < 0.000001f)
			ay = 0.000001f;
		if (ay > XM_PI - 1e-6)
			ay = XM_PI - 1e-6;

		direction.x = sinf(ay) * cosf(ax);
		direction.y = cosf(ay);
		direction.z = sinf(ay) * sinf(ax);
	}
	XMMATRIX Camera::getViewMatrix()
	{
		return m_View;
	}

	XMMATRIX Camera::getProjectionMatrix()
	{
		return m_Projection;
	}

	void Camera::correctSpeed()
	{
		if (speed.x < -maxSpeed) speed.x = -maxSpeed; if (speed.x > maxSpeed) speed.x = maxSpeed;
		if (speed.y < -maxSpeed) speed.y = -maxSpeed; if (speed.y > maxSpeed) speed.y = maxSpeed;
		if (speed.z < -maxSpeed) speed.z = -maxSpeed; if (speed.z > maxSpeed) speed.z = maxSpeed;
	}

	void Camera::setInit()
	{
		XMVECTOR pos_ = XMVectorSet(position.x, position.y, position.z, 0.0f);
		XMVECTOR at_ = XMVectorSet(position.x + direction.x, position.y + direction.y, position.z + +direction.z, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		speed = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	//	ax = getAngle(direction.x, direction.z);

		ay = acosf(direction.y);

		direction.x = sinf(ay) * cosf(ax);
		direction.y = cosf(ay);
		direction.z = sinf(ay) * sinf(ax);

		invertY = false;
		aspectRatio = 1.33333f;

		m_View = XMMatrixLookAtLH(pos_, at_, up);
		m_Projection = XMMatrixPerspectiveFovLH(zoomNow, aspectRatio, 0.1f, 1000.0f);

		m_baseView = XMMatrixLookAtLH(pos_, at_, up);

		moved = false;
	}

}