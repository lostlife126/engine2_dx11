#pragma once
#include "basicStructs.h"


#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "buffer.h"
#include "shader.h"

namespace MyEngine
{
	// ��������� ����� � �����������, ���������� ����������� � ��������
	struct VertexData
	{
		XMFLOAT3  pos;
		XMFLOAT2  tex;
		XMFLOAT3 norm;
	};
	// ��������� �� obj ����� ����� �����, ��������, �������� � ������
	void loadObjInfo(const char* path, int& nVertices, int& nTexels, int& nNormals, int& nFaces);

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
		// ��������� ����� �� ����� � ��������� � ������
		void load(ID3D11Device* device, const char* mesh_path, bool invert = false);
		// ���������� ������
		void release();
		// ����������
		void render(ID3D11DeviceContext* deviceContext);

		ID3D11Buffer* p_vBuff = nullptr; // ����� ������
		ID3D11Buffer* p_iBuff = nullptr;// ����� ��������

	protected:
		// �������� obj ����� � ��� �������
		void loadObj(const char* path, bool invert);
	};

	// ������ ����������� ������� �� �����
	class Object
	{
	public:
		// ����������� �� ��������� (������ � ����������� 0,0,0)
		Object():
			typeMesh(0),
			typeTexture(0)
		{
			m_position.x = 0.0;
			m_position.y = 0.0;
			m_position.z = 0.0;
			m_worldMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}
		// ����������� � ������������ ������ � ������� � ������������ ���������� xyz
		Object(int typeMesh_, int typeTexture_, float x, float y, float z):
			typeMesh(typeMesh_),
			typeTexture(typeTexture_)
		{
			m_position.x = x;
			m_position.y = y;
			m_position.z = z;
			m_worldMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}
		// �������� ������� ������� �������
		XMMATRIX getWorldMatrix()
		{
			return m_worldMatrix;
		}

		int typeMesh; // ��� ����� (�� �������)
		int typeTexture;//  ��� �������� (�� �������)
	private:

		XMFLOAT3 m_position; // ������� ���������
		XMMATRIX m_worldMatrix; // ������� ������� �������
	};

}