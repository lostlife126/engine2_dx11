#pragma once

#include <vector>
#include"basicStructs.h"
#include"log.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include<d3dx10math.h>

namespace MyEngine
{

	struct MatrixBufferType
	{
		XMMATRIX m_World;
		XMMATRIX m_View;
		XMMATRIX m_Projection;
	};

	// класс шейдер в котором прячутся вершинные и пиксельные шейдеры
	class Shader
	{
	public:

		ID3D11PixelShader* m_pShader; // пиксельный шейдер
		ID3D11VertexShader* m_vShader; // вершинный шейдер
		ID3D11InputLayout* m_layout; // формат данных шейдера
		ID3D11Buffer* m_matrixBuffer; // буфер матриц (константный буфер)
		ID3D11SamplerState* m_sampleState;

		// инициализация вершинного и пиксельного шейдеров
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
		{
			HRESULT hr;
			ID3D10Blob* error;

			// вершинный шейдер
			ID3D10Blob* vShaderBuff; // буфер для него
			hr = compileShaderFromFile("mesh_vs.fx", "VS", "vs_4_0", &vShaderBuff); // компилим вершинный файл mesh_vs.fx
			// создаем шейдер
			hr = device->CreateVertexShader(vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), NULL, &m_vShader);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error creating vertex shader.");
			}

			// пиксельный шейдер аналогично
			ID3D10Blob* pShaderBuff;
			hr = compileShaderFromFile("mesh_ps.fx", "PS", "ps_4_0", &pShaderBuff);
			hr = device->CreatePixelShader(pShaderBuff->GetBufferPointer(), pShaderBuff->GetBufferSize(), NULL, &m_pShader);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error creating vertex shader.");
			}

			D3D11_INPUT_ELEMENT_DESC layout[3];

			layout[0].SemanticName = "POSITION";
			layout[0].SemanticIndex = 0;
			layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			layout[0].InputSlot = 0;
			layout[0].AlignedByteOffset = 0;
			layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout[0].InstanceDataStepRate = 0;

			layout[1].SemanticName = "TEXCOORD";
			layout[1].SemanticIndex = 0;
			layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			layout[1].InputSlot = 0;
			layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout[1].InstanceDataStepRate = 0;

			layout[2].SemanticName = "NORMAL";
			layout[2].SemanticIndex = 0;
			layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			layout[2].InputSlot = 0;
			layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout[2].InstanceDataStepRate = 0;


			UINT numElements = ARRAYSIZE(layout);

			hr = device->CreateInputLayout(layout, numElements, vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), &m_layout);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error creating vertex layout.");
			}
			vShaderBuff->Release(); vShaderBuff = nullptr;
			pShaderBuff->Release(); pShaderBuff = nullptr;

			D3D11_BUFFER_DESC md;
			md.Usage = D3D11_USAGE_DYNAMIC;
			md.ByteWidth = sizeof(MatrixBufferType);
			md.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			md.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			md.MiscFlags = 0;
			md.StructureByteStride = 0;

			hr = device->CreateBuffer(&md, NULL, &m_matrixBuffer);
			if (FAILED(hr))
			{
				Log::Get()->Error("Can't create constant buffer.");
			}

			D3D11_SAMPLER_DESC sd;
			sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sd.MipLODBias = 0.0f;
			sd.MaxAnisotropy = 1;
			sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			sd.BorderColor[0] = 0;
			sd.BorderColor[1] = 0;
			sd.BorderColor[2] = 0;
			sd.BorderColor[3] = 0;
			sd.MinLOD = 0;
			sd.MaxLOD = D3D11_FLOAT32_MAX;

			hr = device->CreateSamplerState(&sd, &m_sampleState);
			if (FAILED(hr))
			{
				Log::Get()->Error("Can't create sampler state.");
			}


			return;
		}

		// метод для загрузки шейдера из файла в ID3DBlob
		// filename - имя файла из которого грузим
		// entryPoint - точка в файле с которой начинается шейдер
		// shaderModel - версия шейдера
		// ppBlobOut - буфер куда кладем загруженный шейдер
		HRESULT compileShaderFromFile(const char* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
		{
			HRESULT hr = S_OK;
			// TODO - глянь зачем нужен этот флаг
			DWORD flagShaders = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG )
			flagShaders |= D3DCOMPILE_DEBUG;
#endif
			ID3DBlob* pErrorBlob;
			hr = D3DX11CompileFromFileA(filename, NULL, NULL, entryPoint, shaderModel, flagShaders, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
			if (FAILED(hr) && pErrorBlob != NULL)
			{
				Log::Get()->Error("Error compiling shader: ", (char*)pErrorBlob->GetBufferPointer());
				pErrorBlob->Release(); pErrorBlob = nullptr;
			}

			return hr;
		}
	
		void renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
		{
			deviceContext->IASetInputLayout(m_layout);
			deviceContext->VSSetShader(m_vShader, NULL, 0);
			deviceContext->PSSetShader(m_pShader, NULL, 0);

			deviceContext->PSSetSamplers(0, 1, &m_sampleState);
			deviceContext->DrawIndexed(indexCount, 0, 0);
			return;
		}

		void setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* tex)
		{
			HRESULT hr;
			D3D11_MAPPED_SUBRESOURCE mappedRes;
			MatrixBufferType* p_data;
			unsigned int bufferNum = 0;

			worldMatrix = XMMatrixTranspose(worldMatrix);
			viewMatrix = XMMatrixTranspose(viewMatrix);
			projectionMatrix = XMMatrixTranspose(projectionMatrix);

			hr = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
			p_data = (MatrixBufferType*)mappedRes.pData;
			p_data->m_World = worldMatrix;
			p_data->m_View = viewMatrix;
			p_data->m_Projection = projectionMatrix;
			deviceContext->Unmap(m_matrixBuffer, 0);
			deviceContext->VSSetConstantBuffers(bufferNum, 1, &m_matrixBuffer);

			deviceContext->PSSetShaderResources(0, 1, &tex);
			return;
		}

		void render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* tex)
		{
			setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, tex);
			renderShader(deviceContext, indexCount);
		}
};

}