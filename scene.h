#pragma once

#include <vector>
#include <queue>
#include "camera.h"
#include "model.h"
#include "light.h"
#include "videoDriverDX11.h"

namespace MyEngine
{

	class Frustrum
	{

	public:
		XMVECTOR plane[6];

		void* operator new(size_t i)
		{
			return _aligned_malloc(i, 16);
		}

		void operator delete(void* p)
		{
			_aligned_free(p);
		}

		void constructPlanes(float screenDepth, Camera* camera)
		{
			XMMATRIX proj = camera->getProjectionMatrix();
			float minZ = proj._43 / camera->getProjectionMatrix()._33;
			float r = screenDepth / (screenDepth - minZ);
			proj._33 = r;
			proj._43 = -r * minZ;
			XMMATRIX mat = XMMatrixMultiply(camera->getViewMatrix(), proj);
			plane[0].m128_f32[0] = mat._14 + mat._13; // near
			plane[0].m128_f32[1] = mat._24 + mat._23;
			plane[0].m128_f32[2] = mat._34 + mat._33;
			plane[0].m128_f32[3] = mat._44 + mat._43;
			plane[0] = XMVector4Normalize(plane[0]);

			plane[1].m128_f32[0] = mat._14 - mat._13; // far
			plane[1].m128_f32[1] = mat._24 - mat._23;
			plane[1].m128_f32[2] = mat._34 - mat._33;
			plane[1].m128_f32[3] = mat._44 - mat._43;
			plane[1] = XMVector4Normalize(plane[1]);

			plane[2].m128_f32[0] = mat._14 + mat._11; // left
			plane[2].m128_f32[1] = mat._24 + mat._21;
			plane[2].m128_f32[2] = mat._34 + mat._31;
			plane[2].m128_f32[3] = mat._44 + mat._41;
			plane[2] = XMVector4Normalize(plane[2]);

			plane[3].m128_f32[0] = mat._14 - mat._11; // right
			plane[3].m128_f32[1] = mat._24 - mat._21;
			plane[3].m128_f32[2] = mat._34 - mat._31;
			plane[3].m128_f32[3] = mat._44 - mat._41;
			plane[3] = XMVector4Normalize(plane[3]);

			plane[4].m128_f32[0] = mat._14 + mat._12; // top
			plane[4].m128_f32[1] = mat._24 + mat._22;
			plane[4].m128_f32[2] = mat._34 + mat._32;
			plane[4].m128_f32[3] = mat._44 + mat._42;
			plane[4] = XMVector4Normalize(plane[4]);

			plane[5].m128_f32[0] = mat._14 - mat._12; // bottom
			plane[5].m128_f32[1] = mat._24 - mat._22;
			plane[5].m128_f32[2] = mat._34 - mat._32;
			plane[5].m128_f32[3] = mat._44 - mat._42;
			plane[5] = XMVector4Normalize(plane[5]);
		}

		bool isPointIn(XMFLOAT3 p, XMMATRIX mat)
		{
		//	return true;
			XMVECTOR pv = XMVectorSet(p.x, p.y, p.z, 1.0f);
			pv = XMVector4Transform(pv, mat);
			if (XMPlaneDotCoord(plane[0], pv).m128_f32[0] < 0.0)
				return false;
			if (XMPlaneDotCoord(plane[1], pv).m128_f32[0] > 0.0)
				return false;
			if (XMPlaneDotCoord(plane[2], pv).m128_f32[0] < 0.0)
				return false;
			if (XMPlaneDotCoord(plane[3], pv).m128_f32[0] < 0.0)
				return false;
			if (XMPlaneDotCoord(plane[4], pv).m128_f32[0] < 0.0)
				return false;
			if (XMPlaneDotCoord(plane[5], pv).m128_f32[0] < 0.0)
				return false;
			return true;
		}

		bool isAABBIn(BoxAABB* box, Camera* cam, XMMATRIX pos)
		{
			XMMATRIX mat = pos;

			for (int i = 0; i < 8; i++)
			{
				if (isPointIn(box->pos[i], mat))
				{
					return true;
				}
			}
			return false;
		}
	};

	class Scene
	{
	public:

		void* operator new(size_t i)
		{
			return _aligned_malloc(i, 16);
		}
		// удаляем тоже выровненные
		void operator delete(void* p)
		{
			_aligned_free(p);
		}

		Scene();
		~Scene();

		void init(VideoDriverDX11* driver);

		void update(float dt);
		void drawAll();

		void addObject();

		Camera* getCamera();
		void loadGraph(ID3D11Device* device, const char* mesh_path, const char* tex_path, bool invert = true);

		Light* getLight();
		Frustrum frustrum;

	private:

		Object* skybox;
		Camera* m_camera;
		std::vector<Light*> light;
		std::vector<Mesh*> mesh;
		std::vector<ModelShader*> m_shader;
		VideoDriverDX11* driver;
		std::vector<Object*> object;
		std::list<Object*> visible_objects;

		void frustrumCulling();
	};




}