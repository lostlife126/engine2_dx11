#pragma once
#include "basicStructs.h"
#include"log.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "buffer.h"
#include "shader.h"

namespace MyEngine
{

	struct VertexData
	{
		XMFLOAT3  pos;
		XMFLOAT2  tex;
		XMFLOAT3 norm;
	};

	class BoxAABB
	{
	public:
		float xl, xr, dx;
		float yl, yr, dy;
		float zl, zr, dz;

		void setZero()
		{
			xl = 0.0f; xr = 0.0f; dx = 0.0f;
			yl = 0.0f; yr = 0.0f; dy = 0.0f;
			zl = 0.0f; zr = 0.0f; dz = 0.0f;
		}

		void fill(std::vector<XMFLOAT3>& v)
		{
			setZero();
			for (int i = 0; i < v.size(); i++)
			{
				xl = (xl < v[i].x) ? xl : v[i].x;
				xr = (xr > v[i].x) ? xr : v[i].x;
				yl = (yl < v[i].y) ? yl : v[i].y;
				yr = (yr > v[i].y) ? yr : v[i].y;
				zl = (zl < v[i].z) ? zl : v[i].z;
				zr = (zr > v[i].z) ? zr : v[i].z;
			}
			dx = xr - xl;
			dy = yr - yl;
			dz = zr - zl;
		}
	};

	void loadObjInfo(const char* path, int& nVertices, int& nTexels, int& nNormals, int& nFaces);

	// класс реализующий 3d модель объекта - сетку (вершины нормали индексы и т.д.) и текстуры
	class Model
	{
	public:

		int numVertices;
		int numIndices;
		int numFaces;

		std::string caption;

		std::vector<VertexData> vertices;
		std::vector<DWORD> indices;

		ModelShader* m_shader;
		BoxAABB box;

		void loadObj(const char* path)
		{

			bool invert = true;
			std::vector<XMFLOAT3> v;
			std::vector<XMFLOAT2> t;
			std::vector<XMFLOAT3> n;
			
			std::vector<DWORD> indv;
			std::vector<DWORD> indt;
			std::vector<DWORD> indn;

			int num_v = 0;
			int num_n = 0;
			int num_t = 0;
			int num_f = 0;

			loadObjInfo(path, num_v, num_t, num_n, num_f);
			numVertices = num_t;
			numFaces = num_f;
			numIndices = num_f * 3;

			vertices.resize(num_t);
			indices.resize(numIndices);

			v.reserve(num_v);
			n.reserve(num_n);
			t.reserve(num_t);
			indv.reserve(numIndices);
			indn.reserve(numIndices);
			indt.reserve(numIndices);

			std::ifstream file;
			file.open(path);
			if (!file.good())
			{
				Log::Get()->Error("Error opening file: ", path);
				return;
			}

			std::string line, key, x, y, z;
			char delim = '/';
			while (!file.eof())
			{
				std::getline(file, line);
				std::istringstream iss(line);
				key = "";
				iss >> key;
				if (key == "v") // vertex
				{
					XMFLOAT3 temp;
					iss >> temp.x >> temp.y >> temp.z;
					if (invert)
					{
						temp.z = -temp.z;
					}
					v.push_back(temp);
					continue;
				}
				if (key == "vt") // texture
				{
					XMFLOAT2 temp;
					iss >> temp.x >> temp.y;
					if (invert)
					{
						temp.y = 1.0f - temp.y;
					}
					t.push_back(temp);
					continue;
				}
				if (key == "vn") // normals
				{
					XMFLOAT3 temp;
					iss >> temp.x >> temp.y >> temp.z;
					if (invert)
					{
						temp.z = -temp.z;
					}
					n.push_back(temp);
					continue;
				}
				if (key == "f") // indices of face
				{
					iss >> x >> y >> z;
					std::vector<int> splitX;
					std::vector<int> splitY;
					std::vector<int> splitZ;
					if (invert)
					{
						splitStr(z, delim, splitX);
						splitStr(y, delim, splitY);
						splitStr(x, delim, splitZ);
					}
					else
					{
						splitStr(x, delim, splitX);
						splitStr(y, delim, splitY);
						splitStr(z, delim, splitZ);
					}

					indv.push_back(splitX[0] - 1);
					indv.push_back(splitY[0] - 1);
					indv.push_back(splitZ[0] - 1);

					indt.push_back(splitX[1] - 1);
					indt.push_back(splitY[1] - 1);
					indt.push_back(splitZ[1] - 1);

					indn.push_back(splitX[2] - 1);
					indn.push_back(splitY[2] - 1);
					indn.push_back(splitZ[2] - 1);

					continue;
				}
				if (key.c_str()[0] == '#') // comment
				{
					continue;
				}
				if (key == "o") // caption
				{
					iss >> caption;
					continue;
				}
				if (key == "mtllib") // 
				{
					continue;
				}
			}
			file.close();
			for (int i = 0; i < numFaces; i++)
			{
				vertices[indt[i * 3    ]].pos = v[indv[i * 3    ]];
				vertices[indt[i * 3 + 1]].pos = v[indv[i * 3 + 1]];
				vertices[indt[i * 3 + 2]].pos = v[indv[i * 3 + 2]];

				vertices[indt[i * 3    ]].tex = t[indt[i * 3    ]];
				vertices[indt[i * 3 + 1]].tex = t[indt[i * 3 + 1]];
				vertices[indt[i * 3 + 2]].tex = t[indt[i * 3 + 2]];

				vertices[indt[i * 3    ]].norm = n[indn[i * 3    ]];
				vertices[indt[i * 3 + 1]].norm = n[indn[i * 3 + 1]];
				vertices[indt[i * 3 + 2]].norm = n[indn[i * 3 + 2]];
				
				indices[3 * i    ] = indt[3 * i    ];
				indices[3 * i + 1] = indt[3 * i + 1];
				indices[3 * i + 2] = indt[3 * i + 2];
			}
			box.fill(v);
			return;
		}

		Model()
		{
			m_World = XMMatrixIdentity();
		}

		void load(ID3D11Device* device, const char* mesh_path, const char* tex_path)
		{
			loadObj(mesh_path);
		
			p_vBuff = Buffer::createVertexBuffer(device, sizeof(VertexData) * numVertices, &(vertices[0]), false);
			p_iBuff = Buffer::createIndexBuffer(device, sizeof(DWORD) * numIndices, &(indices[0]), false);

			m_shader = new ModelShader;
			m_shader->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
			m_shader->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
			m_shader->addInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
			m_shader->initShaders(device, "mesh_vs.fx", "mesh_ps.fx");
			m_shader->loadTexture(device, "chest_albedo.png");
		}

		void release()
		{
			if (p_vBuff != nullptr)
			{
				p_vBuff->Release();
				p_vBuff = nullptr;
			}

			if (p_iBuff != nullptr)
			{
				p_iBuff->Release();
				p_iBuff = nullptr;
			}

		}

		void renderBuffers(ID3D11DeviceContext* deviceContext)
		{
			unsigned int stride = sizeof(VertexData);
			unsigned int offset = 0;

			deviceContext->IASetVertexBuffers(0, 1, &p_vBuff, &stride, &offset);
			deviceContext->IASetIndexBuffer(p_iBuff, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			return;
		}

		void render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
		{
			// ????
			renderBuffers(deviceContext);
			// пишем входящие данные в шейдер
			m_shader->setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
			//
			m_shader->draw(deviceContext);
			deviceContext->DrawIndexed(numIndices, 0, 0);
		}

		ID3D11Buffer* p_vBuff = nullptr;
		ID3D11Buffer* p_iBuff = nullptr;


		bool created = false;

		XMMATRIX m_World;



	private:


	};


}