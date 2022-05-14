#pragma once
#include "basicStructs.h"


#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "buffer.h"
#include "shader.h"

namespace MyEngine
{
	// структура точки с координатой, текстурной координатой и нормалью
	struct VertexData
	{
		XMFLOAT3  pos;
		XMFLOAT2  tex;
		XMFLOAT3 norm;
	};
	// загрузить из obj файла число точек, текселей, нормалей и граней
	void loadObjInfo(const char* path, int& nVertices, int& nTexels, int& nNormals, int& nFaces);

	// класс реализующий 3d модель объекта - сетку (вершины нормали индексы и т.д.)
	class Mesh
	{
	public:

		int numVertices; // число вершин
		int numIndices; // число индексов
		int numFaces; // число граней

		std::string caption; // название сетки

		std::vector<VertexData> vertices; // массив с вертексами
		std::vector<DWORD> indices; // массив с индексами
		// кнструктор по умолчанию
		Mesh()
		{
		}
		// загрузить сетку из файла и поместить в буферы
		void load(ID3D11Device* device, const char* mesh_path, bool invert = false);
		// освободить память
		void release();
		// отрисовать
		void render(ID3D11DeviceContext* deviceContext);

		ID3D11Buffer* p_vBuff = nullptr; // буфер вершин
		ID3D11Buffer* p_iBuff = nullptr;// буфер индексов

	protected:
		// загрузка obj файла и его парсинг
		void loadObj(const char* path, bool invert);
	};

	// объект реализующий объекты на сцене
	class Object
	{
	public:
		// конструктор по умолчанию (объект в координатах 0,0,0)
		Object():
			typeMesh(0),
			typeTexture(0)
		{
			m_position.x = 0.0;
			m_position.y = 0.0;
			m_position.z = 0.0;
			m_worldMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}
		// конструктор с определенной сеткой и шкуркой в определенной координате xyz
		Object(int typeMesh_, int typeTexture_, float x, float y, float z):
			typeMesh(typeMesh_),
			typeTexture(typeTexture_)
		{
			m_position.x = x;
			m_position.y = y;
			m_position.z = z;
			m_worldMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}
		// получить мировую матрицу объекта
		XMMATRIX getWorldMatrix()
		{
			return m_worldMatrix;
		}

		int typeMesh; // тип сетки (из массива)
		int typeTexture;//  тип текстуры (из массива)
	private:

		XMFLOAT3 m_position; // текущее положение
		XMMATRIX m_worldMatrix; // мировая матрица объекта
	};

}