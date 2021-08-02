#include "renderer.h"

namespace MyEngine
{
	void Renderer::renderScene(float dt, double time)
	{

		Scene* scene = sceneManager->getScene();
		scene->camera.render(dt);
	//	std::queue<Model*>* obj = scene->getVisibleObjects();
		Model* obj = scene->allObjects[0];
		driverDX11->beginScene();
		int ind = 0;
	//	while (!obj->empty())
	//	{

			obj->render(driverDX11->m_pContextImmediate, obj->m_World, scene->camera.m_View, scene->camera.m_Projection,
				scene->light[0]->lightDir, scene->light[0]->lightAmbient, scene->light[0]->lightDiffuse);
			
	//	}
		
	//	shader.render(driverDX11->m_pContextImmediate, obj->numFaces * 3, obj->m_World, scene->camera.m_View, scene->camera.m_Projection, obj->texture,
	//		scene->light[0]->lightDir, scene->light[0]->lightAmbient, scene->light[0]->lightDiffuse);

     //  text.

			std::stringstream ss;
			ss <<"fps:"<< 1.0f/dt;
			std::string str;
			ss >> str;
		text_fps->setText(driverDX11->m_pContextImmediate, str.c_str());
		text_fps->render(driverDX11->m_pContextImmediate, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

		std::stringstream ss2;
		ss2 <<"time:"<< time;
		ss2 >> str;

		text_time->setText(driverDX11->m_pContextImmediate, str.c_str());
		text_time->render(driverDX11->m_pContextImmediate, 1.0f, 1.0f, 1.0f, 0.0f, 100.0f);

		driverDX11->endScene();


		return;
	}

	void Renderer::init(SceneManager* sm, HWND hwnd_)
	{
		hwnd = hwnd_;
		sceneManager = sm;
		driverDX11 = new VideoDriverDX11;
		driverDX11->init(hwnd);
	//	shader.initShaders(driverDX11->m_pDevice, "mesh.vs", "mesh.ps");

		font.init(driverDX11->m_pDevice, driverDX11->width_screen, driverDX11->height_screen);
		text_fps = new Text(&font);
		text_fps->init(driverDX11->m_pDevice, "fps = ", 15, true);

		text_time = new Text(&font);
		text_time->init(driverDX11->m_pDevice, "time = ", 20, true);

	//	renderer->m_pCamera = sm->getScene()->getCamera();
	//	renderer->setBuffers(1024, 768);
		return;
	}
}