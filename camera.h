#pragma once
//#include "basicStructs.h"
#include <d3d11.h>
//#include <d3dx11.h>

#include <xnamath.h>
#include "inputManager.h"

namespace MyEngine
{

	const float maxSpeed = 50.0;
	const float acceleration = 5.0;

	// класс камеры. инкапсулирует управление камерой

	class Camera : public InputListener
	{
	public:
		// конструктор по умолчанию (в положение 0,0,0, и направление по z )
		Camera() :
			position(0.0, 0.0, 0.0, 0.0),
			speed(0.0, 0.0, 0.0, 0.0),
			direction(0.0, 0.0, 1.0, 0.0),
			zoomNow(1.0),
			invertY(false)

		{
			setInit();
		}

		void* operator new(size_t i)
		{
			return _aligned_malloc(i, 16);
		}
		// удаляем тоже выровненные
		void operator delete(void* p)
		{
			_aligned_free(p);
		}
		
		// если нажата кнопка (здесь управление wasd)
		bool KeyPressed(const KeyEvent& arg) override;
		// если двигается мышь то подвигать камерой (если мышь заблокирована)
		bool MouseMove(const MouseEventMove& arg, bool lockedMouse) override;
		// если крутить колесо то меняется зум (в полтора раза)
		bool MouseWheel(const MouseEventWheel& arg) override;
		
		// поставить камеру в координаты x y z, и направлением по z
		Camera(float x, float y, float z) :
			position(x, y, z, 0.0),
			speed(0.0, 0.0, 0.0, 0.0),
			direction(0.0, 0.0, 1.0, 0.0),
			zoomNow(1.0),
			invertY(false)

		{
			setInit();
		}
		// отработать движение камеры (поменять координаты и перестроить матрицу вида)
		void render(float dt);
		// получить видовую мастрицу
		XMMATRIX getViewMatrix();
		// получить проекционную матрицу
		XMMATRIX getProjectionMatrix();

	private:

		XMMATRIX m_View;// видовая матрица
		XMMATRIX m_Projection; // проекционная матрица
		XMMATRIX m_baseView; // единичная матрица для отложенного рендеринга

		XMFLOAT4 position;// текущее положение
		XMFLOAT4 speed; // текущая скорость
		XMFLOAT4 direction; // текущее направление

		float aspectRatio; // аспектное соотношение экрана
		float ax;// текущий угол поворота в плоскости xz (рысканье)
		float ay; // текущий угол по оси у (крен)
		float zoomNow; // текущий зум
		bool invertY; // инвертировать движение по оси y
		bool moved; // движется ликамера сейчас?
		// есть команда движение вперед
		void moveForward();
		// есть команда движение назад
		void moveBack();
		// есть команда движение влево
		void moveLeft();
		// есть команда движение вправо
		void moveRight();
		// зум на множитель mul 
		void zoom(float mul);
		// поворачивать камерой
		void turn(float dx, float dy);
		// коррекция скорости (не больше максимальной)
		void correctSpeed();
		// инициализация камеры
		void setInit();

	};

}