#pragma once
#include "scene.h"
#include <d3d11.h>
#include <d3dx11.h>
namespace MyEngine
{

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		void update();
		void init(ID3D11Device* deviceContext);
		void close();
		Scene* getScene();

	private:
		Scene* scene;
	};
}