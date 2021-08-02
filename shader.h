#pragma once

#include <vector>
#include"basicStructs.h"
#include"log.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include<d3dx10math.h>
#include "buffer.h"

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

	// ����� ������ � ������� �������� ��������� � ���������� �������

	class Shader
	{
	public:

		ID3D11PixelShader* m_pShader; // ���������� ������
		ID3D11VertexShader* m_vShader; // ��������� ������
		ID3D11InputLayout* m_layout; // ������ ������ �������

		ID3D11SamplerState* m_sampleState;

		D3D11_INPUT_ELEMENT_DESC* m_layout_format;
		ID3D11ShaderResourceView* m_texture = nullptr;


		const int maxLayout = 8;
		int numLayout = 0;

		void addInputElement(const char* name, DXGI_FORMAT format)
		{
			if (numLayout == 0)
			{
				m_layout_format = new D3D11_INPUT_ELEMENT_DESC[maxLayout];
			}

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

		// ������������� ���������� � ����������� ��������
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
		{
			HRESULT hr;
			ID3D10Blob* error;

			// ��������� ������
			ID3D10Blob* vShaderBuff; // ����� ��� ����
			compileShaderFromFile(vShaderFile, "VS", "vs_4_0", &vShaderBuff); // �������� ��������� ���� 
			// ������� ������
			hr = device->CreateVertexShader(vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), NULL, &m_vShader);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error creating vertex shader.");
			}

			// ���������� ������ ����������
			ID3D10Blob* pShaderBuff;
			compileShaderFromFile(pShaderFile, "PS", "ps_4_0", &pShaderBuff);
			hr = device->CreatePixelShader(pShaderBuff->GetBufferPointer(), pShaderBuff->GetBufferSize(), NULL, &m_pShader);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error creating pixel shader.");
			}

			hr = device->CreateInputLayout(m_layout_format, numLayout, vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), &m_layout);
			if (FAILED(hr))
			{
				Log::Get()->Error("Error creating vertex layout.");
			}
			vShaderBuff->Release(); vShaderBuff = nullptr;
			pShaderBuff->Release(); pShaderBuff = nullptr;
			return;
		}

		void loadTexture(ID3D11Device* device, const char* filename)
		{
			HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, filename, NULL, NULL, &m_texture, NULL);
			if (FAILED(hr))
			{
				Log::Get()->Error("Can't create texture from file.");
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
		}

		// ����� ��� �������� ������� �� ����� � ID3DBlob
		// filename - ��� ����� �� �������� ������
		// entryPoint - ����� � ����� � ������� ���������� ������
		// shaderModel - ������ �������
		// ppBlobOut - ����� ���� ������ ����������� ������
		void compileShaderFromFile(const char* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
		{
			HRESULT hr = S_OK;
			// TODO - ����� ����� ����� ���� ����
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

			return;
		}
	
		void draw(ID3D11DeviceContext* deviceContext)
		{
			deviceContext->IASetInputLayout(m_layout);
			deviceContext->VSSetShader(m_vShader, NULL, 0);
			deviceContext->PSSetShader(m_pShader, NULL, 0);
			if (m_texture)
				deviceContext->PSSetShaderResources(0, 1, &m_texture);
			if (m_sampleState)
				deviceContext->PSSetSamplers(0, 1, &m_sampleState);
			return;
		}
};

}