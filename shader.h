#pragma once

#include <vector>
#include"basicStructs.h"
#include"log.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "buffer.h"
#include "Light.h"

namespace MyEngine
{

	struct MatrixBufferType
	{
		XMMATRIX m_World;
		XMMATRIX m_View;
		XMMATRIX m_Projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;
	};

	// общий класс шейдера

	class Shader
	{
	public:

		ID3D11VertexShader* m_vShader = nullptr; // вершинный шейдер
		ID3D11PixelShader* m_pShader = nullptr; // пиксельный шейдер
		ID3D11InputLayout* m_layout = nullptr; // формат данных шейдера

		ID3D11SamplerState* m_sampleState = nullptr;

		D3D11_INPUT_ELEMENT_DESC* m_layout_format = nullptr;
		ID3D11ShaderResourceView* m_texture = nullptr;

		ID3D11Buffer* m_matrixBuffer = nullptr; // константный буфер матрицы

		const int maxLayout = 8;
		int numLayout = 0;

		void addInputElement(const char* name, DXGI_FORMAT format)
		{
			if (numLayout == 0)
				m_layout_format = new D3D11_INPUT_ELEMENT_DESC[maxLayout];

			D3D11_INPUT_ELEMENT_DESC& layout = m_layout_format[numLayout];
			layout.SemanticName = name;
			layout.SemanticIndex = 0;
			layout.Format = format;
			layout.InputSlot = 0;
			if (numLayout == 0)
				layout.AlignedByteOffset = 0;
			else
				layout.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout.InstanceDataStepRate = 0;
			numLayout++;
			return;
		}

		void loadTexture(ID3D11Device* device, const char* filename)
		{
			HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, filename, NULL, NULL, &m_texture, NULL);
			if (FAILED(hr))
				Log::Get()->Error("Can't create texture from file.");
			else
				Log::Get()->Debug("Created texture from file.");

			setSampleState(device);

		}

		void setSampleState(ID3D11Device* device)
		{
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

			HRESULT hr = device->CreateSamplerState(&sd, &m_sampleState);
			if (FAILED(hr))
				Log::Get()->Error("Can't create sampler state.");
			else
				Log::Get()->Debug("Created sampler state.");
		}

		// метод для загрузки шейдера из файла в ID3DBlob
		// filename - имя файла из которого грузим
		// entryPoint - точка в файле с которой начинается шейдер
		// shaderModel - версия шейдера
		// ppBlobOut - буфер куда кладем загруженный шейдер
		void compileShaderFromFile(const char* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
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
				Log::Get()->Error("Error compiling shader:", (char*)pErrorBlob->GetBufferPointer());
				pErrorBlob->Release(); pErrorBlob = nullptr;
			}

			return;
		}
	};

	class ModelShader : public Shader
	{
	public:

		bool setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
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
			deviceContext->PSSetShaderResources(0, 1, &m_texture);
			return true;
		}

		void render(ID3D11DeviceContext* deviceContext, int numIndices, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
		{
			bool res = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
			if (!res)
			{
				return;
			}

			deviceContext->IASetInputLayout(m_layout);
			deviceContext->VSSetShader(m_vShader, NULL, 0);
			deviceContext->PSSetShader(m_pShader, NULL, 0);
					if (m_texture)
						deviceContext->PSSetShaderResources(0, 1, &m_texture);//////// надо ли это тут????
			if (m_sampleState)
				deviceContext->PSSetSamplers(0, 1, &m_sampleState);
			deviceContext->DrawIndexed(numIndices, 0, 0);
			return;
		}

		// инициализация вершинного и пиксельного шейдеров
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
		{
			HRESULT hr;
			ID3D10Blob* error;

			// вершинный шейдер
			ID3D10Blob* vShaderBuff; // буфер для него
			compileShaderFromFile(vShaderFile, "VS", "vs_4_0", &vShaderBuff); // компилим вершинный файл 
			if (vShaderBuff == nullptr)
				Log::Get()->Error("Error reading file vertex shader.");
			// создаем шейдер
			hr = device->CreateVertexShader(vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), NULL, &m_vShader);
			if (FAILED(hr))
				Log::Get()->Error("Error creating vertex shader.");
			else
				Log::Get()->Debug("Created vertex shader.");

			// пиксельный шейдер аналогично
			ID3D10Blob* pShaderBuff;
			compileShaderFromFile(pShaderFile, "PS", "ps_4_0", &pShaderBuff);
			if (pShaderBuff == nullptr)
				Log::Get()->Error("Error reading file pixel shader.");
			hr = device->CreatePixelShader(pShaderBuff->GetBufferPointer(), pShaderBuff->GetBufferSize(), NULL, &m_pShader);
			if (FAILED(hr))
				Log::Get()->Error("Error creating pixel shader.");
			else
				Log::Get()->Debug("Created pixel shader.");

			hr = device->CreateInputLayout(m_layout_format, numLayout, vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), &m_layout);
			if (FAILED(hr))
				Log::Get()->Error("Error creating vertex layout.");
			else
				Log::Get()->Debug("Created vertex layout.");

			vShaderBuff->Release(); vShaderBuff = nullptr;
			pShaderBuff->Release(); pShaderBuff = nullptr;

			m_matrixBuffer = Buffer::createConstantBuffer(device, sizeof(MatrixBufferType), true);

			return;
		}

	};


	
	class LightShader : public Shader
	{
	public:

		ID3D11Buffer* m_lightBuffer;

		void setShaderParameters
		(
			ID3D11DeviceContext* deviceContext,
			XMMATRIX worldMatrix,
			XMMATRIX viewMatrix,
			XMMATRIX projectionMatrix,
			ID3D11ShaderResourceView* textureDiff,
			ID3D11ShaderResourceView* textureNorm,
			XMFLOAT3 lightDir,
			XMFLOAT4 lightAmbient,
			XMFLOAT4 lightDiffuse
		)
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
			deviceContext->PSSetShaderResources(0, 1, &textureDiff);
			deviceContext->PSSetShaderResources(1, 1, &textureNorm);
			
			D3D11_MAPPED_SUBRESOURCE mappedResL;
			LightBufferType* p_dataL;
			
			hr = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResL);
			p_dataL = (LightBufferType*)mappedResL.pData;
			p_dataL->ambient = lightAmbient;
			p_dataL->diffuse = lightDiffuse;
			p_dataL->direction = lightDir;
			deviceContext->Unmap(m_lightBuffer, 0);
			bufferNum = 0;
			deviceContext->PSSetConstantBuffers(bufferNum, 1, &m_lightBuffer);
			
		}

		void createConstantBuffer(ID3D11Device* device)
		{
			m_matrixBuffer = Buffer::createConstantBuffer(device, sizeof(MatrixBufferType), true);
			m_lightBuffer = Buffer::createConstantBuffer(device, sizeof(LightBufferType), true);
			return;
		}


		// инициализация вершинного и пиксельного шейдеров
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
		{
			HRESULT hr;
			ID3D10Blob* error;

			// вершинный шейдер
			ID3D10Blob* vShaderBuff; // буфер для него
			compileShaderFromFile(vShaderFile, "VS", "vs_4_0", &vShaderBuff); // компилим вершинный файл 
			if (vShaderBuff == nullptr)
				Log::Get()->Error("Error reading file vertex shader.");
			// создаем шейдер
			hr = device->CreateVertexShader(vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), NULL, &m_vShader);
			if (FAILED(hr))
				Log::Get()->Error("Error creating vertex shader.");
			else
				Log::Get()->Debug("Created vertex shader.");

			// пиксельный шейдер аналогично
			ID3D10Blob* pShaderBuff;
			compileShaderFromFile(pShaderFile, "PS", "ps_4_0", &pShaderBuff);
			if (pShaderBuff == nullptr)
				Log::Get()->Error("Error reading file pixel shader.");
			hr = device->CreatePixelShader(pShaderBuff->GetBufferPointer(), pShaderBuff->GetBufferSize(), NULL, &m_pShader);
			if (FAILED(hr))
				Log::Get()->Error("Error creating pixel shader.");
			else
				Log::Get()->Debug("Created pixel shader.");

			hr = device->CreateInputLayout(m_layout_format, numLayout, vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), &m_layout);
			if (FAILED(hr))
				Log::Get()->Error("Error creating vertex layout.");
			else
				Log::Get()->Debug("Created vertex layout.");

			vShaderBuff->Release(); vShaderBuff = nullptr;
			pShaderBuff->Release(); pShaderBuff = nullptr;

			m_matrixBuffer = Buffer::createConstantBuffer(device, sizeof(MatrixBufferType), true);
			m_lightBuffer = Buffer::createConstantBuffer(device, sizeof(LightBufferType), true);

			return;
		}

		void render
		(
			ID3D11DeviceContext* deviceContext,
			int numIndices,
			XMMATRIX worldMatrix,
			XMMATRIX viewMatrix,
			XMMATRIX projectionMatrix,
			ID3D11ShaderResourceView* textureDiff,
			ID3D11ShaderResourceView* textureNorm,
			Light* light
		)
		{
			setShaderParameters(deviceContext, worldMatrix, 
				viewMatrix, projectionMatrix, textureDiff, textureNorm, light->lightDir, light->lightAmbient, light->lightDiffuse);

			deviceContext->IASetInputLayout(m_layout);
			deviceContext->VSSetShader(m_vShader, NULL, 0);
			deviceContext->PSSetShader(m_pShader, NULL, 0);
			if (m_sampleState)
				deviceContext->PSSetSamplers(0, 1, &m_sampleState);
			deviceContext->DrawIndexed(numIndices, 0, 0); 
			return;
		}
		
	};
	

}