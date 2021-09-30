#pragma once

#include <vector>
#include <queue>
#include "camera.h"
#include "model.h"
#include "light.h"

#include "videoDriverDX11.h"

namespace MyEngine
{

	class Scene
	{
	public:

		Scene();
		~Scene();

		void init(VideoDriverDX11* driver);

		void update(float dt);
		void drawAll();

		void addObject(const char* pathMesh, const char* pathTexture);

		CameraDX11* getCamera();

	private:
		CameraDX11* m_camera;
		std::vector<Light*> light;
		std::vector<Model*> allObjects;
		VideoDriverDX11* driver;

		void frustrumCulling();
	};




}