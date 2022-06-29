#pragma once
#include "basicStructs.h"


#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "buffer.h"
#include "shader.h"

namespace MyEngine
{
	// hei ��� ������
	// temp - �����������
	// humid - ������
	class Terrain
	{
	public:
		float temp = 0.0; // ����� �� -32 �� 32 ��� 64 / 256 = 0.25;       temp  = 0.25 * char
		float hei = 0.0;  // ����� �� -2048 �� 2048 ��� 4096 / 256 = 16.0; hei   = 16.0 * char
		float humid = 1000.0; // ��������������� ����� �� 62.5 �� 16000:      humid = 1000.0 * exp (char / 32.0 * log(2.0)) 
		int get()
		{
			int tempI = int(temp * 4.0) + 128;
			int heiI = int(hei / 16.0) + 128;
			int humidI = int(log(humid / 1000.0) / log(2.0) * 32.0) + 128;
			if (humidI > 255)
				tempI = tempI;
			return (humidI * 256 + heiI) * 256 + tempI;
		}

		void set(int p)
		{
			temp = (p % 256 - 128) * 0.25;
			p << 8;
			hei = (p % 256 - 128) * 16.0;
			p << 8;
			humid = 1000.0 * exp(p / 32.0 * log(2.0));
		}

		void set(float temp_, float hei_, float humid_)
		{
			temp = (temp_ - 128.0) * 0.25;
			hei = (hei_ - 128.0) * 16.0;
			humid = 1000.0 * exp((humid_ - 128.0) / 32.0 * log(2.0));
		}
	};

	// ��������� ����� � �����������, ���������� ����������� � ��������
	struct VertexData
	{
		XMFLOAT3  pos;
		XMFLOAT2  tex;
		XMFLOAT3 norm;
		XMFLOAT3 tang;
		XMFLOAT3 binorm;
	};
	// ��������� �� obj ����� ����� �����, ��������, �������� � ������
	static bool loadObjInfo(const char* path, int& nVertices, int& nTexels, int& nNormals, int& nFaces);

	// ����� ����������� 3d ������ ������� - ����� (������� ������� ������� � �.�.)
	class Mesh
	{
	public:

		int numVertices; // ����� ������
		int numIndices; // ����� ��������
		int numFaces; // ����� ������

		std::string caption; // �������� �����

		std::vector<VertexData> vertices; // ������ � ����������
		std::vector<DWORD> indices; // ������ � ���������
		// ���������� �� ���������
		Mesh()
		{
		}

		~Mesh()
		{
			release();
		}

		void createBuffers(ID3D11Device* device);
		// ��������� ����� �� ����� � ��������� � ������
		void load(ID3D11Device* device, const char* mesh_path, bool invert = false);
		// ���������� ������
		void release();
		// ����������
		void render(ID3D11DeviceContext* deviceContext);

		void createRectan(std::vector<float>& hei, std::vector<int>& types, int nNodesX, int nNodesY);

		void createWater(XMFLOAT2 size);

		ID3D11Buffer* p_vBuff = nullptr; // ����� ������
		ID3D11Buffer* p_iBuff = nullptr;// ����� ��������

	protected:
		// �������� obj ����� � ��� �������
		bool loadObj(const char* path, bool invert);

		void calcNormals();
	};

	// ������ ����������� ������� �� �����


	class BoxAABB
	{
	public:
		Mesh* p_mesh;
		XMFLOAT3 pos[8];

		BoxAABB(Mesh* mesh):
			p_mesh(mesh)
		{
		}

		void rescale()
		{
			double xMin = 1e10;
			double yMin = 1e10;
			double zMin = 1e10;
			double xMax = -1e10;
			double yMax = -1e10;
			double zMax = -1e10;
			for (int i = 0; i < p_mesh->vertices.size(); i++)
			{
				if (p_mesh->vertices[i].pos.x < xMin) xMin = p_mesh->vertices[i].pos.x;
				if (p_mesh->vertices[i].pos.x > xMax) xMax = p_mesh->vertices[i].pos.x;
				if (p_mesh->vertices[i].pos.y < yMin) yMin = p_mesh->vertices[i].pos.y;
				if (p_mesh->vertices[i].pos.y > yMax) yMax = p_mesh->vertices[i].pos.y;
				if (p_mesh->vertices[i].pos.z < zMin) zMin = p_mesh->vertices[i].pos.z;
				if (p_mesh->vertices[i].pos.z > zMax) zMax = p_mesh->vertices[i].pos.z;
			}
			pos[0].x = xMin; pos[0].y = yMin; pos[0].z = zMin;
			pos[1].x = xMax; pos[1].y = yMin; pos[1].z = zMin;
			pos[2].x = xMax; pos[2].y = yMin; pos[2].z = zMax;
			pos[3].x = xMin; pos[3].y = yMin; pos[3].z = zMax;
			pos[4].x = xMin; pos[4].y = yMax; pos[4].z = zMin;
			pos[5].x = xMax; pos[5].y = yMax; pos[5].z = zMin;
			pos[6].x = xMax; pos[6].y = yMax; pos[6].z = zMax;
			pos[7].x = xMin; pos[7].y = yMax; pos[7].z = zMax;
		}

	};

	class Object
	{
	public:
		// ����������� �� ��������� (������ � ����������� 0,0,0)
		Object() :
			typeMesh(0),
			typeTexture(0)
		{
			m_position.x = 0.0;
			m_position.y = 0.0;
			m_position.z = 0.0;
			m_worldMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}
		// ����������� � ������������ ������ � ������� � ������������ ���������� xyz
		Object(int typeMesh_, int typeTexture_, float x, float y, float z) :
			typeMesh(typeMesh_),
			typeTexture(typeTexture_)
		{
			m_position.x = x;
			m_position.y = y;
			m_position.z = z;
			m_worldMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}

		void setBox(Mesh* mesh)
		{
			box = new BoxAABB(mesh);
			box->rescale();
		}
		// �������� ������� ������� �������
		XMMATRIX getWorldMatrix()
		{
			return m_worldMatrix;
		}

		void frame()
		{
			box->rescale();
		}

		int typeMesh; // ��� ����� (�� �������)
		int typeTexture;//  ��� �������� (�� �������)
		BoxAABB* box;
	private:

		XMFLOAT3 m_position; // ������� ���������
		XMMATRIX m_worldMatrix; // ������� ������� �������
	};

}