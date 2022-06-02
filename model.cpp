#include "model.h"
#include"log.h"
#include <xnamath.h>

namespace MyEngine
{

	XMFLOAT2 texType[4] =
	{
		XMFLOAT2(0.5, 0.0), // rocks
		XMFLOAT2(1.0, 0.0), // water
		XMFLOAT2(0.5, 0.5), // sand
		XMFLOAT2(1.0, 0.5) // grass
	};

	void loadObjInfo(const char* path, int& nVertices, int& nTexels, int& nNormals, int& nFaces)
	{
		nVertices = 0;
		nTexels = 0;
		nNormals = 0;
		nFaces = 0;
		std::ifstream file;
		file.open(path);
		if (!file.good())
		{
			Log::Get()->Error("Error opening file: ", path);
			return;
		}
		std::string line, key;
		while (!file.eof())
		{
			std::getline(file, line);
			std::istringstream iss(line);
			key = "";
			iss >> key;
			if (key == "v") // vertex
			{
				nVertices++;
				continue;
			}
			if (key == "vn") // vertex
			{
				nNormals++;
				continue;
			}
			if (key == "vt") // vertex
			{
				nTexels++;
				continue;
			}
			if (key == "f") // vertex
			{
				nFaces++;
				continue;
			}
		}
		file.close();
		return;
	}

	void Mesh::createRectan(std::vector<float>& hei, std::vector<int>& types, int nNodesX, int nNodesY)
	{
	//	int nNodes = nNodesX * nNodesY;
		int nCells = (nNodesX - 1) * (nNodesY - 1);
		numFaces = nCells * 2;
		numVertices = numFaces * 3;
		numIndices = numVertices;

		vertices.resize(numVertices);
		indices.resize(numIndices);
		int indVer = 0;
		int indCell = 0;
		float sizeTex = 0.5;
		float coefHei = 0.002; // коэффициент для нормировки высоты - макс высота - 2.048 метра
		XMFLOAT2 pos(-nNodesX * 0.5, -nNodesY * 0.5);
		for (int j = 0; j < nNodesY  - 1; j++)
		{
			pos.x = -nNodesX * 0.5;
			for (int i = 0; i < nNodesX - 1; i++)
			{ // по часовой стрелке!!!!!!!!!!!
				int indNodeLB = i + j * nNodesX;
				int indNodeRB = i + 1 + j * nNodesX;
				int indNodeLT = i + (j + 1) * nNodesX;
				int indNodeRT = i + 1 + (j + 1) * nNodesX;

				vertices[indVer].pos.x = pos.x;
				vertices[indVer].pos.y = hei[indNodeLB] * coefHei;
				vertices[indVer].pos.z = pos.y;
				vertices[indVer].tex = texType[types[indCell]];
				indices[indVer] = indVer;
				indVer++;

				vertices[indVer].pos.x = pos.x + 1.0;
				vertices[indVer].pos.y = hei[indNodeRB] *coefHei;
				vertices[indVer].pos.z = pos.y;
				vertices[indVer].tex = texType[types[indCell]];
				vertices[indVer].tex.x -= sizeTex;
				indices[indVer] = indVer;
				indVer++;

				vertices[indVer].pos.x = pos.x;
				vertices[indVer].pos.y = hei[indNodeLT] * coefHei;;
				vertices[indVer].pos.z = pos.y + 1.0;
				vertices[indVer].tex = texType[types[indCell]];
				vertices[indVer].tex.y += sizeTex;
				indices[indVer] = indVer;
				indVer++;

				vertices[indVer].pos.x = pos.x + 1.0;
				vertices[indVer].pos.y = hei[indNodeRB] * coefHei;;
				vertices[indVer].pos.z = pos.y;
				vertices[indVer].tex = texType[types[indCell]];
				vertices[indVer].tex.x -= sizeTex;
				indices[indVer] = indVer;
				indVer++;

				vertices[indVer].pos.x = pos.x + 1.0;
				vertices[indVer].pos.y = hei[indNodeRT] * coefHei;;
				vertices[indVer].pos.z = pos.y + 1.0;
				vertices[indVer].tex = texType[types[indCell]];
				vertices[indVer].tex.x -= sizeTex;
				vertices[indVer].tex.y += sizeTex;
				indices[indVer] = indVer;
				indVer++;

				vertices[indVer].pos.x = pos.x;
				vertices[indVer].pos.y = hei[indNodeLT] * coefHei;;
				vertices[indVer].pos.z = pos.y + 1.0;
				vertices[indVer].tex = texType[types[indCell]];
				vertices[indVer].tex.y += sizeTex;
				indices[indVer] = indVer;
				indVer++;
				pos.x += 1.0;
				indCell++;
			}
			pos.y += 1.0;
		}
		calcNormals();
	}

	void Mesh::loadObj(const char* captionFile, bool invert)
	{

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
		std::string filename = captionFile;
		filename += "_mesh.obj";
		loadObjInfo(filename.c_str(), num_v, num_t, num_n, num_f);
		numVertices = num_f * 3; // num_t
		numFaces = num_f;
		numIndices = num_f * 3;

		vertices.resize(numVertices);
		indices.resize(numIndices);

		v.reserve(num_v);
		n.reserve(num_n);
		t.reserve(num_t);
		indv.reserve(numIndices);
		indn.reserve(numIndices);
		indt.reserve(numIndices);

		std::ifstream file;
		file.open(filename);
		if (!file.good())
		{
			Log::Get()->Error("Error opening file: ", filename);
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
					temp.z = -temp.z;
				v.push_back(temp);
				continue;
			}
			if (key == "vt") // texture
			{
				XMFLOAT2 temp;
				iss >> temp.x >> temp.y;
				if (invert)
					temp.y = 1.0f - temp.y;
				t.push_back(temp);
				continue;
			}
			if (key == "vn") // normals
			{
				XMFLOAT3 temp;
				iss >> temp.x >> temp.y >> temp.z;
				if (invert)
					temp.z = -temp.z;
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
			if (key == "mtllib") // описание материалов
			{
				continue;
			}
		}
		file.close();
		for (int i = 0; i < numFaces; i++)
		{	
			vertices[i * 3].pos     = v[indv[i * 3]];
			vertices[i * 3 + 1].pos = v[indv[i * 3 + 1]];
			vertices[i * 3 + 2].pos = v[indv[i * 3 + 2]];

			vertices[i * 3].tex     = t[indt[i * 3]];
			vertices[i * 3 + 1].tex = t[indt[i * 3 + 1]];
			vertices[i * 3 + 2].tex = t[indt[i * 3 + 2]];

			vertices[i * 3].norm     = n[indn[i * 3]];
			vertices[i * 3 + 1].norm = n[indn[i * 3 + 1]];
			vertices[i * 3 + 2].norm = n[indn[i * 3 + 2]];

			indices[3 * i] = 3 * i;
			indices[3 * i + 1] = 3 * i + 1;
			indices[3 * i + 2] = 3 * i + 2;
		}
		calcNormals();

		return;
	}

	void Mesh::calcNormals()
	{
		for (int i = 0; i < numFaces; i++)
		{
			XMFLOAT3 v1;
			XMFLOAT3 v2;
			XMFLOAT2 t1;
			XMFLOAT2 t2;

			auto& p1 = vertices[indices[3 * i]];
			auto& p2 = vertices[indices[3 * i + 1]];
			auto& p3 = vertices[indices[3 * i + 2]];

			v1.x = p2.pos.x - p1.pos.x;
			v1.y = p2.pos.y - p1.pos.y;
			v1.z = p2.pos.z - p1.pos.z;

			v2.x = p3.pos.x - p1.pos.x;
			v2.y = p3.pos.y - p1.pos.y;
			v2.z = p3.pos.z - p1.pos.z;

			t1.x = p2.tex.x - p1.tex.x;
			t1.y = p2.tex.y - p1.tex.y;

			t2.x = p3.tex.x - p1.tex.x;
			t2.y = p3.tex.y - p1.tex.y;

			double den = 1.0f / (t1.x * t2.y - t2.x * t1.y);

			p1.tang.x = (t2.y * v1.x - t1.y * v2.x) * den;
			p1.tang.y = (t2.y * v1.y - t1.y * v2.y) * den;
			p1.tang.z = (t2.y * v1.z - t1.y * v2.z) * den;

			p1.binorm.x = (t1.x * v2.x - t2.x * v1.x) * den;
			p1.binorm.y = (t1.x * v2.y - t2.x * v1.y) * den;
			p1.binorm.z = (t1.x * v2.z - t2.x * v1.z) * den;

			double len_r = 1.0 / sqrt(sqr(p1.tang.x) + sqr(p1.tang.y) + sqr(p1.tang.z));
			p1.tang.x *= len_r;
			p1.tang.y *= len_r;
			p1.tang.z *= len_r;

			len_r = 1.0 / sqrt(sqr(p1.binorm.x)	+ sqr(p1.binorm.y) + sqr(p1.binorm.z));
			p1.binorm.x *= len_r;
			p1.binorm.y *= len_r;
			p1.binorm.z *= len_r;

			p1.norm.x = p1.tang.y * p1.binorm.z - p1.tang.z * p1.binorm.y;
			p1.norm.y = p1.tang.z * p1.binorm.x - p1.tang.x * p1.binorm.z;
			p1.norm.z = p1.tang.x * p1.binorm.y - p1.tang.y * p1.binorm.x;

			len_r = 1.0 / sqrt(sqr(p1.norm.x) + sqr(p1.norm.y) + sqr(p1.norm.z));

			p1.norm.x *= len_r;
			p1.norm.y *= len_r;
			p1.norm.z *= len_r;

			p3.tang = p2.tang = p1.tang;

			p3.binorm = p2.binorm = p1.binorm;

			double dot = p1.norm.x * p2.norm.x + p1.norm.y * p2.norm.y + p1.norm.z * p2.norm.z;
			if (dot < 0.0)
				dot = dot;
			p3.norm = p2.norm = p1.norm;
		}
	}

	void Mesh::load(ID3D11Device* device, const char* caption, bool invert)
	{
		loadObj(caption, invert);
		p_vBuff = Buffer::createVertexBuffer(device, sizeof(VertexData) * numVertices, &(vertices[0]), false);
		p_iBuff = Buffer::createIndexBuffer(device, sizeof(DWORD) * numIndices, &(indices[0]), false);
	}

	void Mesh::createBuffers(ID3D11Device* device)
	{
		p_vBuff = Buffer::createVertexBuffer(device, sizeof(VertexData) * numVertices, &(vertices[0]), false);
		p_iBuff = Buffer::createIndexBuffer(device, sizeof(DWORD) * numIndices, &(indices[0]), false);
	}



	void Mesh::release()
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

	void Mesh::render(ID3D11DeviceContext* deviceContext)
	{
		unsigned int stride = sizeof(VertexData);
		unsigned int offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &p_vBuff, &stride, &offset);
		deviceContext->IASetIndexBuffer(p_iBuff, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return;
	}
}