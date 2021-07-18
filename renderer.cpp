#include "renderer.h"

namespace MyEngine
{
	void Renderer::renderScene()
	{

		Scene* scene = sceneManager->getScene();
		scene->camera.render(0.01f);
	//	std::queue<Model*>* obj = scene->getVisibleObjects();
		Model* obj = scene->allObjects[0];
		driverDX11->beginScene();
		int ind = 0;
	//	while (!obj->empty())
	//	{

			obj->render(driverDX11->m_pContextImmediate);
			
	//	}
		
		shader.render(driverDX11->m_pContextImmediate, obj->numFaces * 3, obj->m_World, scene->camera.m_View, scene->camera.m_Projection, obj->texture);

		driverDX11->endScene();


		return;
	}

	void Renderer::init(SceneManager* sm, HWND hwnd_)
	{
		hwnd = hwnd_;
		sceneManager = sm;
		driverDX11 = new VideoDriverDX11;
		driverDX11->init(hwnd);
		shader.initShaders(driverDX11->m_pDevice, "mesh.vs", "mesh.ps");
	//	renderer->m_pCamera = sm->getScene()->getCamera();
	//	renderer->setBuffers(1024, 768);
		return;
	}
}