#pragma once
#include <queue>
#include "sceneManager.h"
#include "videoDriverDX11.h"
#include "shader.h"

namespace MyEngine
{
	class Renderer
	{
	public:
		void renderScene();
		void init(SceneManager* sm, HWND hwnd_);
		VideoDriverDX11* driverDX11;
		SceneManager* sceneManager;
		Shader shader;

	private:
		


		int m_width = 1024;
		int m_height = 768;
		HWND hwnd;

	};
}