#include "scene.h"
namespace MyEngine
{

	Camera* Scene::getCamera()
	{
		return m_camera;
	}

	void Scene::update(float dt, float timeDay)
	{
		m_camera->update(dt);
		
	//	light[0]->update(timeDay);
		sun->update(timeDay);
		return;
	}

	Light* Scene::getLight()
	{
	//	return light[0];
		return static_cast<Light*>(sun);
	}

	void Scene::init(VideoDriverDX11* driver_)
	{
		driver = driver_;
		m_camera = new Camera(0.0, 2.0, -2.0);
		//region.setRegion("map.bmp");
		region.init(driver->getDevice());
		region.typeTexture = 1;
		loadModel(driver->getDevice(), "skybox");
		loadModel(driver->getDevice(), "terrain");
		loadModel(driver->getDevice(), "chest", true);
		loadModel(driver->getDevice(), "cube");
		loadModel(driver->getDevice(), "firehydrant", true);

		meshes.push_back(new Mesh);
		meshes.back()->createWater(XMFLOAT2(100.0, 100.0));
		meshes.back()->createBuffers(driver->getDevice());

		addObjects();
	//	light.push_back(new Light);
		sun = new Sun;
		transparentShader = new ModelShader;
		transparentShader->initShaders(driver->getDevice(), "water.vs", "water.ps");
		transparentShader->loadTextures(driver->getDevice(), "water");

		shadowShader = new ShadowShader;

		shadowShader->initShaders(driver->getDevice(), "shadows.vs", "shadows.ps");
		shadowShader->setSampleState(driver->getDevice());
		return;
	}

	void Scene::loadModel(ID3D11Device* device, const char* caption, bool invert)
	{
		meshes.push_back(new Mesh);
		meshes.back()->load(driver->getDevice(), caption, invert);

		m_shader.push_back(new ModelShader);
		m_shader.back()->initShaders(device, "mesh.vs", "mesh.ps");
		m_shader.back()->loadTextures(device, caption);
	}

	void Scene::addObjects()
	{
		skybox = new Object(0, 0, 0.0, 0.0, 0.0);
		water = new Object(5, 0, 0.0, 0.0, 0.0);
		object.push_back(new Object(2, 2, -10.0, 0.25, 0.0)); // chest
		object.push_back(new Object(3, 3, 10.0, 0.5, 0.0)); // cube
		object.push_back(new Object(4, 4, 10.0, 0.0, 1.0)); // hydrant
		object[0]->setBox(meshes[2]);
		object[1]->setBox(meshes[3]);
		object[2]->setBox(meshes[4]);
		for (int i = 0; i < 50; i++)
		{
			double x = ((rand() % 20000) - 10000) / 1000.0;
			double y = ((rand() % 20000)) / 2000.0;
			double z = ((rand() % 20000) - 10000) / 1000.0;
			object.push_back(new Object(4, 4, x, y, z)); // hydrant
			object.back()->setBox(meshes[3]);
		}
	}

	void Scene::constructFrustrum()
	{
		frustrum.constructPlanes(100.0, m_camera);
	}

	void Scene::drawAllOpaque(ID3D11ShaderResourceView* shadow)
	{

		meshes[skybox->typeMesh]->render(driver->getDeviceContext());
		m_shader[skybox->typeTexture]->render(driver->getDeviceContext(), meshes[skybox->typeMesh]->numIndices, skybox->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), getLight(), shadow);

		region.mesh->render(driver->getDeviceContext());
		m_shader[region.typeTexture]->render(driver->getDeviceContext(), region.mesh->numIndices, skybox->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), getLight(), shadow);

		frustrumCulling();
		for (auto iter = visible_objects.begin(); iter != visible_objects.end();)
		{
			meshes[(*iter)->typeMesh]->render(driver->getDeviceContext());
			m_shader[(*iter)->typeTexture]->render(driver->getDeviceContext(), meshes[(*iter)->typeMesh]->numIndices, (*iter)->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), getLight(), shadow);
			iter = visible_objects.erase(iter);
		}
	}

	void Scene::drawAllOpaqueCam(ID3D11ShaderResourceView* shadow)
	{

		meshes[skybox->typeMesh]->render(driver->getDeviceContext());
		m_shader[skybox->typeTexture]->render(driver->getDeviceContext(), meshes[skybox->typeMesh]->numIndices, skybox->getWorldMatrix(), getLight()->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), getLight(), shadow);

		region.mesh->render(driver->getDeviceContext());
		m_shader[region.typeTexture]->render(driver->getDeviceContext(), region.mesh->numIndices, skybox->getWorldMatrix(), getLight()->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), getLight(), shadow);

		frustrumCulling();
		for (auto iter = visible_objects.begin(); iter != visible_objects.end();)
		{
			meshes[(*iter)->typeMesh]->render(driver->getDeviceContext());
			m_shader[(*iter)->typeTexture]->render(driver->getDeviceContext(), meshes[(*iter)->typeMesh]->numIndices, (*iter)->getWorldMatrix(), getLight()->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), getLight(), shadow);
			iter = visible_objects.erase(iter);
		}
	}

	void Scene::drawAllShadows()
	{

		meshes[skybox->typeMesh]->render(driver->getDeviceContext());
		shadowShader->render(driver->getDeviceContext(), meshes[skybox->typeMesh]->numIndices, skybox->getWorldMatrix(), getLight()->getViewMatrix());

		region.mesh->render(driver->getDeviceContext());
		shadowShader->render(driver->getDeviceContext(), region.mesh->numIndices, skybox->getWorldMatrix(), getLight()->getViewMatrix());

		frustrumCulling();
		for (auto iter = visible_objects.begin(); iter != visible_objects.end();)
		{
			meshes[(*iter)->typeMesh]->render(driver->getDeviceContext());
			shadowShader->render(driver->getDeviceContext(), meshes[(*iter)->typeMesh]->numIndices, (*iter)->getWorldMatrix(), getLight()->getViewMatrix());
			iter = visible_objects.erase(iter);
		}
	}

	void Scene::drawAllTransparent(ID3D11ShaderResourceView* shadow)
	{
		meshes[water->typeMesh]->render(driver->getDeviceContext());
		transparentShader->render(driver->getDeviceContext(), meshes[water->typeMesh]->numIndices, water->getWorldMatrix(), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(), m_camera->getPosition(), getLight(), shadow);
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