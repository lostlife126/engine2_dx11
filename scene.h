#pragma once

#include <vector>
#include <queue>
#include "camera.h"
#include "model.h"
#include "light.h"
#include "videoDriverDX11.h"

namespace MyEngine
{


	class NodeField
	{
	public:

		int width;
		int height;
		std::vector<float> data;

		NodeField(int sizeX, int sizeY):
			width(sizeX),
			height(sizeY)
		{
			data.resize(width * height);
			nulling();
		}

		void nulling()
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] = 0.0;
			}
		}

		void setChains()
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] = 1.0 - 2.0 * fabs(data[i] - 0.5);
			}
		}

		void normalize(float amp = 1.0f)
		{
			float minD = 1e10;
			float maxD = -1e10;
			for (int i = 0; i < data.size(); i++)
			{
				minD = (minD < data[i]) ? minD : data[i];
				maxD = (maxD > data[i]) ? maxD : data[i];
			}
			float rangeD_r = amp / (maxD - minD);
			for (int i = 0; i < data.size(); i++)
			{
				data[i] = (data[i] - minD) * rangeD_r;
			}
		}

		void setExpBorder(float lambda)
		{
			for (int i = 0; i < data.size(); i++)
			{
				int x = i % width;
				float lenBorder = myMin(x, width - x);
				int y = i / width;
				lenBorder = myMax(1.0, myMin(myMin(lenBorder, y), height - y));
				data[i] = data[i] * (1.0 - exp(-lambda * lenBorder));
			}
		}

		void setValue(float x)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] = x;
			}
		}

		void addGradient(float amp, XMFLOAT2 dir)
		{

		}

		void addNoise(float amp, float k)
		{
			int nNodesX = k + 1.9999999;
			int nNodesY = k + 1.9999999;
			std::vector<float> rNodes(nNodesX * nNodesY);
			for (int i = 0; i < rNodes.size(); i++)
			{
				rNodes[i] = getRand();
			}

			for (int j = 0; j < height; j++)
			{
				for (int i = 0; i < width; i++)
				{
					int left = (i * k) / width;
					int right = left + 1;
					int bottom = (j * k) / height;
					int top = bottom + 1;
					int nodeLB = left + nNodesX * bottom;
					int nodeRB = right + nNodesX * bottom;
					int nodeLT = left + nNodesX * top;
					int nodeRT = right + nNodesX * top;
					float dx = (i * k) / width - left;
					float dy = (j * k) / height - bottom;
					data[i + width * j] += rNodes[nodeLB] * (1.0 - dx) * (1.0 - dy) + rNodes[nodeRB] * dx * (1.0 - dy) + rNodes[nodeLT] * (1.0 - dx) * dy + rNodes[nodeRT] * dx * dy;
				}
			}

		}

	};

	class Region
	{
	public:
		Region()
		{}

		void readFile(const char* filename)
		{
			FILE* file;
			BITMAPFILEHEADER bmpFileHeader;
			BITMAPINFOHEADER bmpInfoHeader;

			fopen_s(&file, filename, "rb");
			fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
			fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
			nNodesX = bmpInfoHeader.biWidth;
			nNodesY = bmpInfoHeader.biHeight;

			unsigned char* bitmapBuffer = new unsigned char[bmpInfoHeader.biSizeImage];
			fseek(file, bmpFileHeader.bfOffBits, SEEK_SET);
			fread(bitmapBuffer, 1, bmpInfoHeader.biSizeImage, file);
			fclose(file);

			nodes.resize(nNodesX * nNodesY);

			int offset = 0;
			int posNode = 0;
			for (int j = 0; j < nNodesY; j++)
			{
				double x = pos.x;
				for (int i = 0; i < nNodesX; i++)
				{
					int tem;
					int hei;
					int hum;
					tem = bitmapBuffer[offset];
					hei = bitmapBuffer[offset + 1];
					hum = bitmapBuffer[offset + 2];
					nodes[posNode].set(tem, hei, hum);
					posNode++;
					offset += 3;
				}
			}
			delete[] bitmapBuffer;

		}

		void createMap(int sizeMap)
		{
			NodeField pTemp(sizeMap, sizeMap);
			pTemp.addNoise(1.0, 2.0);

			NodeField pHei(sizeMap, sizeMap);
			pHei.addNoise(1.0, 5.0);
			pHei.setChains();
			pHei.addNoise(0.3, 10.0);
			pHei.addNoise(0.12, 20.0);
			pHei.addNoise(0.05, 50.0);
			pHei.normalize();
			pHei.setExpBorder(0.1);

			NodeField pHum(sizeMap, sizeMap);
			pHum.addNoise(1.0, 2.0);
			nodes.resize(sizeMap * sizeMap);

			for (int i = 0; i < nodes.size(); i++)
			{
				nodes[i].set(pTemp.data[i] * 255.0, pHei.data[i] * 255.0, pHum.data[i] * 255.0);
			}
			nNodesX = sizeMap;
			nNodesY = sizeMap;

			saveToBMP("map1.bmp");
		}

		void saveToBMP(const char* filename)
		{
			int kp = nNodesX * 3 % 4;
			if (kp != 0) kp = 4 - kp;
			int sizeStr = 3 * nNodesX + kp; // с выравниванием на 4 байта


			BITMAPFILEHEADER bmpFileHeader;
			bmpFileHeader.bfType = 19778;
			bmpFileHeader.bfSize = 54 + sizeStr * nNodesY;
			bmpFileHeader.bfOffBits = 54;
			bmpFileHeader.bfReserved1 = 0;
			bmpFileHeader.bfReserved2 = 0;

			BITMAPINFOHEADER bmpInfoHeader;
			bmpInfoHeader.biSize = 40;
			bmpInfoHeader.biWidth = nNodesX;
			bmpInfoHeader.biHeight = nNodesY;
			bmpInfoHeader.biPlanes = 1;
			bmpInfoHeader.biBitCount = 24;
			bmpInfoHeader.biCompression = 0;
			bmpInfoHeader.biSizeImage = sizeStr * nNodesY;
			bmpInfoHeader.biXPelsPerMeter = 0;
			bmpInfoHeader.biYPelsPerMeter = 0;
			bmpInfoHeader.biClrUsed = 0;
			bmpInfoHeader.biClrImportant = 0;

			std::vector<char> buffer(sizeStr);

			FILE* file;
			fopen_s(&file, filename, "wb");

			fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
			fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
			for (int j = 0; j < nNodesY; j++)
			{
				int pos = 0;
				for (int i = 0; i < nNodesX; i++)
				{
					int pix = nodes[i + nNodesX * j].get();
					memcpy(&buffer[pos], &pix, 3);
					pos += 3;
				}
				for (int i = 0; i < kp; i++)
				{
					buffer[pos] = 0;
					pos++;
				}
				fwrite(&(buffer[0]), 1, sizeStr, file);
			}
			fclose(file);


		}

		void init(ID3D11Device* device)
		{
			if (false)
				readFile("map.bmp");
			else
				createMap(100);

			nCellsX = nNodesX - 1;
			nCellsY = nNodesY - 1;
			mesh = new Mesh;
			std::vector<float> heis(nNodesX * nNodesY);
			std::vector<int> cellTypes((nNodesX - 1) * (nNodesY - 1));
			for (int i = 0; i < heis.size(); i++)
			{
				heis[i] = nodes[i].hei;
			}
			int ind = 0;
			for (int j = 0; j < nCellsY; j++)
			{
				for (int i = 0; i < nCellsX; i++)
				{
					int nLB = i + nNodesX * j;
					int nRB = i + 1 + nNodesX * j;
					int nLT = i + nNodesX * (j + 1);
					int nRT = i + 1 + nNodesX * (j + 1);
					cellTypes[ind] = 3;
					if (nodes[nLB].hei < 0.0 || nodes[nRB].hei < 0.0 || nodes[nLT].hei < 0.0 || nodes[nRT].hei < 0.0)
						cellTypes[ind] = 2;
					if(nodes[nLB].hei<0.0 && nodes[nRB].hei < 0.0 && nodes[nLT].hei < 0.0 && nodes[nRT].hei < 0.0)
						cellTypes[ind] = 2;
					if (nodes[nLB].hei > 1000.0 || nodes[nRB].hei > 1000.0 || nodes[nLT].hei > 1000.0 || nodes[nRT].hei > 1000.0)
						cellTypes[ind] = 0;
					ind++;
				}
			}

			mesh->createRectan(heis, cellTypes, nCellsX + 1, nCellsY + 1);
			mesh->createBuffers(device);
		}
		
		int typeTexture;
		Mesh* mesh;
	protected:
		
		std::vector<Terrain> nodes;
		int nNodesX;
		int nNodesY;
		int nCellsX;
		int nCellsY;
		XMFLOAT2 size;
		XMFLOAT2 pos;
	};

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

			int v = 9;
			return !(v & 1);
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

		void update(float dt, float timeDay);
		void drawAllShadows();
		void drawAllOpaque(ID3D11ShaderResourceView* shadow);
		void drawAllOpaqueCam(ID3D11ShaderResourceView* shadow);
		void drawAllTransparent(ID3D11ShaderResourceView* shadow);

		void constructFrustrum();

		void addObjects();

		Camera* getCamera();
		void loadModel(ID3D11Device* device, const char* caption, bool invert = false);

		Light* getLight();
		Frustrum frustrum;

	private:

		Object* skybox;
		Object* water;
		Camera* m_camera;
		Sun* sun;
	//	std::vector<Light*> light;
		std::vector<Mesh*> meshes;
		std::vector<ModelShader*> m_shader;
		VideoDriverDX11* driver;
		std::vector<Object*> object;
		std::list<Object*> visible_objects;
		Region region;
		ModelShader* transparentShader;
		ShadowShader* shadowShader;
		bool stopped = true;

		void frustrumCulling();
	};




}