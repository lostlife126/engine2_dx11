#include "model.h"
#include"log.h"

namespace MyEngine
{

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

	void Mesh::loadObj(const char* path, bool invert)
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
			vertices[indt[i * 3]].pos = v[indv[i * 3]];
			vertices[indt[i * 3 + 1]].pos = v[indv[i * 3 + 1]];
			vertices[indt[i * 3 + 2]].pos = v[indv[i * 3 + 2]];

			vertices[indt[i * 3]].tex = t[indt[i * 3]];
			vertices[indt[i * 3 + 1]].tex = t[indt[i * 3 + 1]];
			vertices[indt[i * 3 + 2]].tex = t[indt[i * 3 + 2]];

			vertices[indt[i * 3]].norm = n[indn[i * 3]];
			vertices[indt[i * 3 + 1]].norm = n[indn[i * 3 + 1]];
			vertices[indt[i * 3 + 2]].norm = n[indn[i * 3 + 2]];

			indices[3 * i] = indt[3 * i];
			indices[3 * i + 1] = indt[3 * i + 1];
			indices[3 * i + 2] = indt[3 * i + 2];
		}
		return;
	}

	void Mesh::load(ID3D11Device* device, const char* mesh_path, bool invert)
	{
		loadObj(mesh_path, invert);
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