#pragma once


#include"basicStructs.h"

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

	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 color;
		XMFLOAT3 dir;
		float specularPower;
	};

	struct ConstantBuffer
	{
		XMMATRIX wvp;
	};

	struct FogLinearType
	{
		float start;
		float end;
		float padding1;
		float padding2;
	};

	struct PixelBufferType
	{
		XMFLOAT4 color;
	};

	// ����� ����� �������

	class Shader
	{
	public:

		// ��������� ������� ������� ��� �������
		// name - ��� ��������� (POSITION, TEXCOORD, NORMAL)
		// ������ ��������� (DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_R32G32_FLOAT)
		void addInputElement(const char* name, DXGI_FORMAT format);

		// ��������� ��������
		virtual void loadTextures(ID3D11Device* device, const char* filename) = 0;

		// �������� ��������� �������� (��� ������� �������)
		void setSampleState(ID3D11Device* device);

	protected:

		ID3D11VertexShader* m_vShader = nullptr; // ��������� ������
		ID3D11PixelShader* m_pShader = nullptr; // ���������� ������
		ID3D11InputLayout* m_layout = nullptr; // ������ ������ �������
		ID3D11SamplerState* m_sampleState = nullptr; // �������
		D3D11_INPUT_ELEMENT_DESC* m_layout_format = nullptr; //  ������ ������� ���������

		const int maxLayout = 8;
		int numLayout = 0;

		// ����� ��� �������� ������� �� ����� � ID3DBlob
		// filename - ��� ����� �� �������� ������
		// entryPoint - ����� � ����� � ������� ���������� ������
		// shaderModel - ������ �������
		// ppBlobOut - ����� ���� ������ ����������� ������
		void compileShaderFromFile(const char* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);
	
		// �������� � ������������� ���������� ������� � ������� ������� ������
		void initVertexShaders(ID3D11Device* device, const char* vShaderFile, const char* entryPoint);
		
		// �������� � ������������� ����������� ������� � ������� ������� ������
		void initPixelShaders(ID3D11Device* device, const char* pShaderFile, const char* entryPoint);

};

	class ModelShader : public Shader
	{
	public:

		void loadTextures(ID3D11Device* device, const char* filename) override;

		void setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPos);

		void render(ID3D11DeviceContext* deviceContext, int numIndices, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPos);

		// ������������� ���������� � ����������� �������� � �������� ������ ������
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile);

		ID3D11ShaderResourceView* m_texture[5]; // albedo, normal, roughness, metallness, ao 
		ID3D11Buffer* m_matrixBuffer = nullptr; // ����������� ����� �������
		ID3D11Buffer* m_cameraBuffer;
	};
	
	class TextShader : public Shader
	{
	public:

		ID3D11Buffer* m_constantBuffer = nullptr;
		ID3D11Buffer* m_pixelBuffer = nullptr;

		void loadTextures(ID3D11Device* device, const char* filename) override;

		void setShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4 color, XMFLOAT2 pos, XMMATRIX m_orthoMatrix);

		void render(ID3D11DeviceContext* deviceContext, int numIndices, XMFLOAT4 color, XMFLOAT2 pos, XMMATRIX projectionMatrix);

			// ������������� ���������� � ����������� �������� � �������� ������ ������
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile);

		ID3D11ShaderResourceView* m_texture; // font

	};
	
	class LightShader : public Shader
	{
	public:
		ID3D11Buffer* m_matrixBuffer = nullptr; // ����������� ����� �������
		ID3D11Buffer* m_lightBuffer;
		ID3D11Buffer* m_cameraBuffer;


		void setShaderParameters
		(
			ID3D11DeviceContext* deviceContext,
			XMMATRIX worldMatrix,
			XMMATRIX viewMatrix,
			XMMATRIX projectionMatrix,
			ID3D11ShaderResourceView* texture[],
			Light* light,
			XMFLOAT3 cameraPos
		);

		void loadTextures(ID3D11Device* device, const char* filename) override
		{}

		// ������������� ���������� � ����������� ��������
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile);

		void render
		(
			ID3D11DeviceContext* deviceContext,
			int numIndices,
			XMMATRIX worldMatrix,
			XMMATRIX viewMatrix,
			XMMATRIX projectionMatrix,
			ID3D11ShaderResourceView* texture[],
			Light* light,
			XMFLOAT3 cameraPos
		);
		
	};
	
	class FogShader : public Shader
	{
	public:

		void loadTextures(ID3D11Device* device, const char* filename) override
		{}

		void setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX viewMatrix);

		void render(ID3D11DeviceContext* deviceContext, XMMATRIX viewMatrix);

		// ������������� ���������� � ����������� �������� � �������� ������ ������
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile);

		ID3D11Buffer* m_matrixBuffer = nullptr; // ����������� ����� �������
	};

}