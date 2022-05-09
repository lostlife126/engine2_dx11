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

		void addObject();

		CameraDX11* getCamera();
		void loadGraph(ID3D11Device* device, const char* mesh_path, const char* tex_path);

		Light* getLight();

	private:
		CameraDX11* m_camera;
		std::vector<Light*> light;
		std::vector<Mesh*> mesh;
		std::vector<ModelShader*> m_shader;
		VideoDriverDX11* driver;
		std::vector<Object*> object;


		void frustrumCulling();
	};




}