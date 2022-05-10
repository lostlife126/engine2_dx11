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
		frustrumCulling();
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
		object.push_back(new Object(0, 0, 0.0, 0.0, 0.0));
		object.push_back(new Object(1, 1, 0.0, 0.0, 0.0));
		object.push_back(new Object(2, 2, -1.0, 0.5, 0.0));
		object.push_back(new Object(3, 3, 1.0, 0.5, 0.0));
		object.push_back(new Object(4, 4, 1.0, 0.5, 1.0));
	}

	void Scene::drawAll()
	{
		for (auto obj : object)
		{
			mesh[obj->typeMesh]->render(driver->getDeviceContext());
			m_shader[obj->typeTexture]->render(driver->getDeviceContext(), mesh[obj->typeMesh]->numIndices, obj->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
		}
	}

	void Scene::frustrumCulling()
	{
		for (int i = 0; i < object.size(); i++)
		{
		//	visibleObjects.push(allObjects[i]);
		}
		return;
	}


	Scene::Scene()
	{}

	Scene::~Scene()
	{}
}