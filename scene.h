#pragma once

#include <vector>
#include <queue>
#include "camera.h"
#include "model.h"

#include <d3d11.h>
#include <d3dx11.h>

namespace MyEngine
{

	class Scene
	{
	public:

		Scene();
		~Scene();

		void update();
		void init(ID3D11Device* deviceContext);

		std::queue<Model*>* getVisibleObjects();
		std::vector<Model*> allObjects;
		CameraDX11 camera;

	private:
		void load();

		std::queue<Model*> visibleObjects;

		void frustrumCulling();
	};




}