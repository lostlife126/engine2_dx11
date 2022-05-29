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

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;
	};

	struct ConstantBuffer
	{
		XMMATRIX wvp;
	};

	struct PixelBufferType
	{
		XMFLOAT4 color;
	};

	// общий класс шейдера

	class Shader
	{
	public:

		// добавл€ем входной элемент дл€ шейдера
		// name - им€ структуры (POSITION, TEXCOORD, NORMAL)
		// формат структуры (DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_R32G32_FLOAT)
		void addInputElement(const char* name, DXGI_FORMAT format);

		// загружаем текстуру
		virtual void loadTextures(ID3D11Device* device, const char* filename) = 0;

		// создание состо€ни€ сэмплера (дл€ выборки текстур)
		void setSampleState(ID3D11Device* device);

	protected:

		ID3D11VertexShader* m_vShader = nullptr; // вершинный шейдер
		ID3D11PixelShader* m_pShader = nullptr; // пиксельный шейдер
		ID3D11InputLayout* m_layout = nullptr; // формат данных шейдера

		ID3D11SamplerState* m_sampleState = nullptr; // сэмплер

		D3D11_INPUT_ELEMENT_DESC* m_layout_format = nullptr;


		ID3D11Buffer* m_matrixBuffer = nullptr; // константный буфер матрицы

		const int maxLayout = 8;
		int numLayout = 0;

		// метод дл€ загрузки шейдера из файла в ID3DBlob
		// filename - им€ файла из которого грузим
		// entryPoint - точка в файле с которой начинаетс€ шейдер
		// shaderModel - верси€ шейдера
		// ppBlobOut - буфер куда кладем загруженный шейдер
		void compileShaderFromFile(const char* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);
	
		// загрузка и инициализаци€ вершинного шейдера и формата входных данных
		void initVertexShaders(ID3D11Device* device, const char* vShaderFile, const char* entryPoint);
		
		// загрузка и инициализаци€ пиксельного шейдера
		void initPixelShaders(ID3D11Device* device, const char* pShaderFile, const char* entryPoint);

};

	class ModelShader : public Shader
	{
	public:

		void loadTextures(ID3D11Device* device, const char* filename) override;

		bool setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

		void render(ID3D11DeviceContext* deviceContext, int numIndices, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

		// инициализаци€ вершинного и пиксельного шейдеров и создание буфера матриц
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile);

		ID3D11ShaderResourceView* m_texture[5]; // diffuse, normal, ao, metal, rough

	};

	
	class TextShader : public Shader
	{
	public:

		ID3D11Buffer* m_constantBuffer = nullptr;
		ID3D11Buffer* m_pixelBuffer = nullptr;

		void loadTextures(ID3D11Device* device, const char* filename) override;

		bool setShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4 color, XMFLOAT2 pos, XMMATRIX m_orthoMatrix);

		void render(ID3D11DeviceContext* deviceContext, int numIndices, XMFLOAT4 color, XMFLOAT2 pos, XMMATRIX projectionMatrix);

			// инициализаци€ вершинного и пиксельного шейдеров и создание буфера матриц
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile);

		ID3D11ShaderResourceView* m_texture; // font

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
		);

		void createConstantBuffer(ID3D11Device* device);

		void loadTextures(ID3D11Device* device, const char* filename) override;

		// инициализаци€ вершинного и пиксельного шейдеров
		void initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile);

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
		);

		ID3D11ShaderResourceView* m_texture; // font
		
	};
	

}