#include "scene.h"
namespace MyEngine
{

	Camera* Scene::getCamera()
	{
		return m_camera;
	}

	void Scene::update(float dt, float timeDay)
	{
		m_camera->render(dt);
		
		light[0]->update(timeDay);
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
		//region.setRegion("map.bmp");
		region.init(driver->getDevice());
		region.typeTexture = 1;
		loadObject(driver->getDevice(), "skybox");
		loadObject(driver->getDevice(), "terrain");
		loadObject(driver->getDevice(), "chest", true);
		loadObject(driver->getDevice(), "cube");
		loadObject(driver->getDevice(), "firehydrant", true);

		mesh.push_back(new Mesh);
		mesh.back()->createWater(XMFLOAT2(100.0, 100.0));
		mesh.back()->createBuffers(driver->getDevice());

		addObjects();
		light.push_back(new Light(0));
		transparentShader = new ModelShader;
		transparentShader->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		transparentShader->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		transparentShader->addInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		transparentShader->addInputElement("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
		transparentShader->addInputElement("BINORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		transparentShader->initShaders(driver->getDevice(), "water.vs", "water.ps");
		transparentShader->loadTextures(driver->getDevice(), "water");

		shadowShader = new ShadowShader;
		shadowShader->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		shadowShader->initShaders(driver->getDevice(), "shadows.vs", "shadows.ps");
		shadowShader->setSampleState(driver->getDevice());
		return;
	}

	void Scene::loadObject(ID3D11Device* device, const char* caption, bool invert)
	{
		mesh.push_back(new Mesh);
		mesh.back()->load(driver->getDevice(), caption, invert);

		m_shader.push_back(new ModelShader);
		m_shader.back()->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader.back()->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		m_shader.back()->addInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader.back()->addInputElement("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader.back()->addInputElement("BINORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader.back()->initShaders(device, "mesh.vs", "mesh.ps");
		m_shader.back()->loadTextures(device, caption);
	}

	void Scene::addObjects()
	{
		skybox = new Object(0, 0, 0.0, 0.0, 0.0);
		water = new Object(5, 0, 0.0, 0.0, 0.0);
	//	object.push_back(new Object(5, 1, 0.0, 0.0, 0.0)); // floor
		object.push_back(new Object(2, 2, -10.0, 0.25, 0.0)); // chest
		object.push_back(new Object(3, 3, 10.0, 0.5, 0.0)); // cube
		object.push_back(new Object(4, 4, 10.0, 0.0, 1.0)); // hydrant
	//	object[0]->setBox(mesh[5]);
		object[0]->setBox(mesh[2]);
		object[1]->setBox(mesh[3]);
		object[2]->setBox(mesh[4]);
		for (int i = 0; i < 50; i++)
		{
			double x = ((rand() % 20000) - 10000) / 1000.0;
			double y = ((rand() % 20000)) / 2000.0;
			double z = ((rand() % 20000) - 10000) / 1000.0;
			object.push_back(new Object(4, 4, x, y, z)); // hydrant
			object.back()->setBox(mesh[3]);
		}
	}

	void Scene::constructFrustrum()
	{
		frustrum.constructPlanes(100.0, m_camera);
	}

	void Scene::drawAllOpaque(ID3D11ShaderResourceView* shadow)
	{
		light[0]->resetViewMatrix();
		mesh[skybox->typeMesh]->render(driver->getDeviceContext());
		m_shader[skybox->typeTexture]->render(driver->getDeviceContext(), mesh[skybox->typeMesh]->numIndices, skybox->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), light[0], shadow);

		region.mesh->render(driver->getDeviceContext());
		m_shader[region.typeTexture]->render(driver->getDeviceContext(), region.mesh->numIndices, skybox->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), light[0], shadow);

		frustrumCulling();
		for (auto iter = visible_objects.begin(); iter != visible_objects.end();)
		{
			mesh[(*iter)->typeMesh]->render(driver->getDeviceContext());
			m_shader[(*iter)->typeTexture]->render(driver->getDeviceContext(), mesh[(*iter)->typeMesh]->numIndices, (*iter)->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), light[0], shadow);
			iter = visible_objects.erase(iter);
		}
	}

	void Scene::drawAllOpaqueCam(ID3D11ShaderResourceView* shadow)
	{
		light[0]->resetViewMatrix();
		mesh[skybox->typeMesh]->render(driver->getDeviceContext());
		m_shader[skybox->typeTexture]->render(driver->getDeviceContext(), mesh[skybox->typeMesh]->numIndices, skybox->getWorldMatrix(), light[0]->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), light[0], shadow);

		region.mesh->render(driver->getDeviceContext());
		m_shader[region.typeTexture]->render(driver->getDeviceContext(), region.mesh->numIndices, skybox->getWorldMatrix(), light[0]->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), light[0], shadow);

		frustrumCulling();
		for (auto iter = visible_objects.begin(); iter != visible_objects.end();)
		{
			mesh[(*iter)->typeMesh]->render(driver->getDeviceContext());
			m_shader[(*iter)->typeTexture]->render(driver->getDeviceContext(), mesh[(*iter)->typeMesh]->numIndices, (*iter)->getWorldMatrix(), light[0]->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), light[0], shadow);
			iter = visible_objects.erase(iter);
		}
	}

	void Scene::drawAllShadows()
	{
		light[0]->resetViewMatrix();
		mesh[skybox->typeMesh]->render(driver->getDeviceContext());
		shadowShader->render(driver->getDeviceContext(), mesh[skybox->typeMesh]->numIndices, skybox->getWorldMatrix(), light[0]->getViewMatrix());

		region.mesh->render(driver->getDeviceContext());
		shadowShader->render(driver->getDeviceContext(), region.mesh->numIndices, skybox->getWorldMatrix(), light[0]->getViewMatrix());

		frustrumCulling();
		for (auto iter = visible_objects.begin(); iter != visible_objects.end();)
		{
			mesh[(*iter)->typeMesh]->render(driver->getDeviceContext());
			shadowShader->render(driver->getDeviceContext(), mesh[(*iter)->typeMesh]->numIndices, (*iter)->getWorldMatrix(), light[0]->getViewMatrix());
			iter = visible_objects.erase(iter);
		}
	}

	void Scene::drawAllTransparent(ID3D11ShaderResourceView* shadow)
	{
		mesh[water->typeMesh]->render(driver->getDeviceContext());
		transparentShader->render(driver->getDeviceContext(), mesh[water->typeMesh]->numIndices, water->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), light[0], shadow);
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