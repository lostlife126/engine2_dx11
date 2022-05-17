#include "scene.h"
namespace MyEngine
{

	Camera* Scene::getCamera()
	{
		return m_camera;
	}

	void Scene::update(float dt)
	{
		m_camera->render(dt);
		frustrum.constructPlanes(100.0, m_camera);
		light[0]->update(dt);
		return;
	}

	Light* Scene::getLight()
	{
		return light[0];
	}

	void Scene::init(VideoDriverDX11* driver_)
	{
		driver = driver_;
		m_camera = new Camera(0.0, 2.0, -2.0);
		loadGraph(driver->getDevice(), "skybox.obj", "skybox.png", true);
		loadGraph(driver->getDevice(), "floor.obj", "floor.bmp");
		loadGraph(driver->getDevice(),"chest_mesh.obj", "chest_albedo.png");
		loadGraph(driver->getDevice(), "cube.obj", "cube.bmp");
		loadGraph(driver->getDevice(), "firehydrant_mesh.obj", "firehydrant_albedo.png");

		addObject();
		light.push_back(new Light(1.0, 1.0, 0.0));
		return;
	}

	void Scene::loadGraph(ID3D11Device* device, const char* mesh_path, const char* tex_path, bool invert)
	{
		mesh.push_back(new Mesh);
		mesh.back()->load(driver->getDevice(), mesh_path, invert);

		m_shader.push_back(new ModelShader);
		m_shader.back()->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader.back()->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		m_shader.back()->addInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader.back()->initShaders(device, "mesh_vs.fx", "mesh_ps.fx");
		m_shader.back()->loadTexture(device, tex_path);
	}

	void Scene::addObject()
	{
		skybox = new Object(0, 0, 0.0, 0.0, 0.0);
		object.push_back(new Object(1, 1, 0.0, 0.0, 0.0)); // floor
		object.push_back(new Object(2, 2, -1.0, 0.25, 0.0)); // chest
		object.push_back(new Object(3, 3, 1.0, 0.5, 0.0)); // cube
		object.push_back(new Object(4, 4, 1.0, 0.0, 1.0)); // hydrant
		object[0]->setBox(mesh[1]);
		object[1]->setBox(mesh[2]);
		object[2]->setBox(mesh[3]);
		object[3]->setBox(mesh[4]);
		for (int i = 0; i < 50; i++)
		{
			double x = ((rand() % 20000) - 10000) / 1000.0;
			double y = ((rand() % 20000)) / 2000.0;
			double z = ((rand() % 20000) - 10000) / 1000.0;
			object.push_back(new Object(4, 4, x, y, z)); // hydrant
			object.back()->setBox(mesh[4]);
		}
	}

	void Scene::drawAll()
	{
		mesh[skybox->typeMesh]->render(driver->getDeviceContext());
		m_shader[skybox->typeTexture]->render(driver->getDeviceContext(), mesh[skybox->typeMesh]->numIndices, skybox->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix());

		frustrumCulling();
		for (auto iter = visible_objects.begin(); iter != visible_objects.end();)
		{
			mesh[(*iter)->typeMesh]->render(driver->getDeviceContext());
			m_shader[(*iter)->typeTexture]->render(driver->getDeviceContext(), mesh[(*iter)->typeMesh]->numIndices, (*iter)->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
			iter = visible_objects.erase(iter);
		}
	}

	void Scene::frustrumCulling()
	{
		for (int i = 0; i < object.size(); i++)
		{
			if(frustrum.isAABBIn(object[i]->box, m_camera, object[i]->getWorldMatrix()))
				visible_objects.push_back(object[i]);
		}
		return;
	}


	Scene::Scene()
	{}

	Scene::~Scene()
	{}
}