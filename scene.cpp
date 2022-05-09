#include "scene.h"
namespace MyEngine
{

	CameraDX11* Scene::getCamera()
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
		m_camera = new CameraDX11;
		loadGraph(driver->getDevice(),"chest_mesh.obj", "chest_albedo2.png");
		addObject();
		light.push_back(new Light);
		return;
	}

	void Scene::loadGraph(ID3D11Device* device, const char* mesh_path, const char* tex_path)
	{
		mesh.push_back(new Mesh);
		mesh.back()->load(driver->getDevice(), mesh_path);

		m_shader.push_back(new ModelShader);
		m_shader[0]->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader[0]->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		m_shader[0]->addInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader[0]->initShaders(device, "mesh_vs.fx", "mesh_ps.fx");
		m_shader[0]->loadTexture(device, "chest_albedo.png");
	}

	void Scene::addObject()
	{
		object.push_back(new Object);
		object[0]->typeMesh = 0;
		object[0]->typeTexture = 0;
		object[0]->position = XMFLOAT3(1.0, 0.0, 0.0);
		object[0]->worldMatrix = XMMatrixTranslation(object[0]->position.x, object[0]->position.y, object[0]->position.z);

		object.push_back(new Object);
		object[1]->typeMesh = 0;
		object[1]->typeTexture = 0;
		object[1]->position = XMFLOAT3(-1.0, 0.0, 0.0);
		object[1]->worldMatrix = XMMatrixTranslation(object[1]->position.x, object[1]->position.y, object[1]->position.z);

	}

	void Scene::drawAll()
	{
		for (auto obj : object)
		{
			mesh[obj->typeMesh]->render(driver->getDeviceContext());
			m_shader[obj->typeTexture]->render(driver->getDeviceContext(), mesh[obj->typeMesh]->numIndices, obj->worldMatrix, m_camera->m_View, m_camera->m_Projection);
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