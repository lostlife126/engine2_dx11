#include "buffer.h"
#include "log.h"

namespace MyEngine
{

	ID3D11Buffer* Buffer::createVertexBuffer(ID3D11Device* device, int size, const void* data, bool dynamic)
	{
		ID3D11Buffer* vb = nullptr;
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = size;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		if (dynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = data;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&bd, &sd, &vb);
		if (FAILED(hr))
		{
			Log::Get()->Error("Error creating vertex buffer!");
		}
		else
		{
			Log::Get()->Debug("Vertex buffer was created.");
		}
		return vb;
	}

	ID3D11Buffer* Buffer::createIndexBuffer(ID3D11Device* device, int size, const void* data, bool dynamic)
	{
		ID3D11Buffer* ib = nullptr;
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = size;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		if (dynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = data;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&bd, &sd, &ib);
		if (FAILED(hr))
		{
			Log::Get()->Error("Error creating index buffer!");
		}
		else
		{
			Log::Get()->Debug("Index buffer was created.");
		}
		return ib;
	}

	ID3D11Buffer* Buffer::createConstantBuffer(ID3D11Device* device, int size, bool dynamic)
	{
		ID3D11Buffer* cb = nullptr;
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = size;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		if (dynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}

		HRESULT hr = device->CreateBuffer(&bd, NULL, &cb);
		if (FAILED(hr))
		{
			Log::Get()->Error("Error creating constant buffer!");
		}
		else
		{
			Log::Get()->Debug("Constant buffer was created.");
		}
		return cb;
	}
}