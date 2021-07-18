#pragma once

#include <vector>
#include"basicStructs.h"
#include"log.h"
#include <d3d11.h>
#include <d3dx11.h>


namespace MyEngine
{
	// chunk is class for incapsulating functions of drawing
	class ChunkDX11
	{
	public:

		std::vector<Vect3f> vertices;
		std::vector<int> indices;
		ID3D11Buffer* p_vBuff = nullptr;
		ID3D11Buffer* p_iBuff = nullptr;
		ID3D11ShaderResourceView* texture = nullptr; 

		void initBuffers(ID3D11Device* device)
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vect3f) * vertices.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA vData;
			ZeroMemory(&vData, sizeof(vData));
			vData.pSysMem = &(vertices[0]);
			HRESULT hr = device->CreateBuffer(&bd, &vData, &p_vBuff);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error buffering vertex.");
			}

			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.ByteWidth = sizeof(int) * indices.size();
			D3D11_SUBRESOURCE_DATA iData;
			ZeroMemory(&iData, sizeof(iData));
			iData.pSysMem = &(indices[0]);
			hr = device->CreateBuffer(&bd, &iData, &p_iBuff);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error buffering index.");
			}
		}

		void setTexture(ID3D11Device* device, std::string filename)
		{
			HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, filename.c_str(), NULL, NULL, &texture, NULL);
			if (FAILED(hr))
			{
				Log::Get()->Error("Can't create texture from file.");
			}
			return;
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

			if (texture != nullptr)
			{
				texture->Release();
				texture = nullptr;
			}
		}

		void renderBuffers(ID3D11DeviceContext* deviceContext)
		{
			unsigned int stride = sizeof(Vect3f);
			unsigned int offset = 0;

			deviceContext->IASetVertexBuffers(0, 1, &p_vBuff, &stride, &offset);
			deviceContext->IASetIndexBuffer(p_iBuff, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			return;
		}

		void render(ID3D11DeviceContext* deviceContext)
		{
			renderBuffers(deviceContext);
		}


	};
}