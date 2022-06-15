#include "shader.h"
#include"log.h"

namespace MyEngine
{
	////////////////////////////////////    Shader   ///////////////////////////////////////////////
	void Shader::addInputElement(const char* name, DXGI_FORMAT format)
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

	void Shader::setSampleState(ID3D11Device* device)
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

	void Shader::compileShaderFromFile(const char* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
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
			Log::Get()->Error("%s %s","Error compiling shader:", (char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release(); pErrorBlob = nullptr;
		}

		return;
	}

	void Shader::initVertexShaders(ID3D11Device* device, const char* vShaderFile, const char* entryPoint)
	{
		HRESULT hr;
		ID3D10Blob* error;

		// вершинный шейдер
		ID3D10Blob* vShaderBuff; // буфер для него
		compileShaderFromFile(vShaderFile, entryPoint, "vs_4_0", &vShaderBuff); // компилим файл вершинного шейдера
		if (vShaderBuff == nullptr)
			Log::Get()->Error("%s %s", "Error reading file vertex shader:", vShaderFile);
		// создаем шейдер
		hr = device->CreateVertexShader(vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), NULL, &m_vShader); // создаем вершинный шейдер
		if (FAILED(hr))
			Log::Get()->Error("Error creating vertex shader.");
		else
			Log::Get()->Debug("Created vertex shader.");

		hr = device->CreateInputLayout(m_layout_format, numLayout, vShaderBuff->GetBufferPointer(), vShaderBuff->GetBufferSize(), &m_layout); // подаем типы выходных данных
		if (FAILED(hr))
			Log::Get()->Error("Error creating vertex layout.");
		else
			Log::Get()->Debug("Created vertex layout.");

		vShaderBuff->Release(); vShaderBuff = nullptr;
	}

	void Shader::initPixelShaders(ID3D11Device* device, const char* pShaderFile, const char* entryPoint)
	{
		HRESULT hr;
		ID3D10Blob* error;

		// пиксельный шейдер аналогично
		ID3D10Blob* pShaderBuff;
		compileShaderFromFile(pShaderFile, entryPoint, "ps_4_0", &pShaderBuff);
		if (pShaderBuff == nullptr)
			Log::Get()->Error("Error reading file pixel shader.");
		hr = device->CreatePixelShader(pShaderBuff->GetBufferPointer(), pShaderBuff->GetBufferSize(), NULL, &m_pShader);
		if (FAILED(hr))
			Log::Get()->Error("Error creating pixel shader.");
		else
			Log::Get()->Debug("Created pixel shader.");

	}

	////////////////////////////////////   Model Shader   ///////////////////////////////////////////////

	void ModelShader::loadTextures(ID3D11Device* device, const char* caption)
	{
		std::string filename;
		filename = caption;
		filename += "_albedo.png";
		HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, filename.c_str(), NULL, NULL, &m_texture[0], NULL);
		if (FAILED(hr))
			Log::Get()->Error("%s %s", "Can't create texture from file:", filename.c_str());
		else
			Log::Get()->Debug("%s %s", "Created texture from file: ", filename.c_str());
		filename = caption;
		filename += "_normal.png";
		hr = D3DX11CreateShaderResourceViewFromFileA(device, filename.c_str(), NULL, NULL, &m_texture[1], NULL);
		if (FAILED(hr))
			Log::Get()->Error("%s %s", "Can't create texture from file:", filename.c_str());
		else
			Log::Get()->Debug("%s %s", "Created texture from file:", filename.c_str());
		filename = caption;
		filename += "_roughness.png";
		hr = D3DX11CreateShaderResourceViewFromFileA(device, filename.c_str(), NULL, NULL, &m_texture[2], NULL);
		if (FAILED(hr))
			Log::Get()->Error("%s %s", "Can't create texture from file:", filename.c_str());
		else
			Log::Get()->Debug("%s %s", "Created texture from file:", filename.c_str());
		filename = caption;
		filename += "_metallness.png";
		hr = D3DX11CreateShaderResourceViewFromFileA(device, filename.c_str(), NULL, NULL, &m_texture[3], NULL);
		if (FAILED(hr))
			Log::Get()->Error("%s %s", "Can't create texture from file:", filename.c_str());
		else
			Log::Get()->Debug("%s %s", "Created texture from file:", filename.c_str());
		filename = caption;
		filename += "_ao.png";
		hr = D3DX11CreateShaderResourceViewFromFileA(device, filename.c_str(), NULL, NULL, &m_texture[4], NULL);
		if (FAILED(hr))
			Log::Get()->Error("%s %s", "Can't create texture from file:", filename.c_str());
		else
			Log::Get()->Debug("%s %s", "Created texture from file:", filename.c_str());

		setSampleState(device);
	}

	void ModelShader::setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPos)
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
		deviceContext->PSSetShaderResources(0, 1, &m_texture[0]);
	//	if(m_texture[1]!=NULL)
			deviceContext->PSSetShaderResources(1, 1, &m_texture[1]);
	//	if (m_texture[2] != NULL)
			deviceContext->PSSetShaderResources(2, 1, &m_texture[2]);
	//	if (m_texture[3] != NULL)
			deviceContext->PSSetShaderResources(3, 1, &m_texture[3]);
	//	if (m_texture[4] != NULL)
			deviceContext->PSSetShaderResources(4, 1, &m_texture[4]);

		D3D11_MAPPED_SUBRESOURCE mappedResC;
		CameraBufferType* p_dataC;

		hr = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResC);
		p_dataC = (CameraBufferType*)mappedResC.pData;
		p_dataC->position = cameraPos;
		deviceContext->Unmap(m_cameraBuffer, 0);
		bufferNum = 1;
		deviceContext->VSSetConstantBuffers(bufferNum, 1, &m_cameraBuffer);

		return;
	}

	void ModelShader::render(ID3D11DeviceContext* deviceContext, int numIndices, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPos)
	{
		setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, cameraPos);

		deviceContext->IASetInputLayout(m_layout);
		deviceContext->VSSetShader(m_vShader, NULL, 0);
		deviceContext->PSSetShader(m_pShader, NULL, 0);
		if (m_sampleState)
			deviceContext->PSSetSamplers(0, 1, &m_sampleState);
		deviceContext->DrawIndexed(numIndices, 0, 0);
		return;
	}

	void ModelShader::initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
	{
		initVertexShaders(device, vShaderFile, "VS");
		initPixelShaders(device, pShaderFile, "PS");
		m_matrixBuffer = Buffer::createConstantBuffer(device, sizeof(MatrixBufferType), true);
		m_cameraBuffer = Buffer::createConstantBuffer(device, sizeof(CameraBufferType), true);
		return;
	}

	////////////////////////////////////   Text Shader   ///////////////////////////////////////////////

	void TextShader::loadTextures(ID3D11Device* device, const char* caption)
	{
		std::string filename;
		filename = caption;
		filename += "_font.png";
		HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, caption, NULL, NULL, &m_texture, NULL);
		if (FAILED(hr))
			Log::Get()->Error("%s %s","Can't create texture from file:", filename);
		else
			Log::Get()->Debug("Created texture from file.");

		setSampleState(device);
	}

	void TextShader::setShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4 color, XMFLOAT2 pos, XMMATRIX m_orthoMatrix)
	{
		XMMATRIX objMatrix = XMMatrixTranslation(pos.x, -pos.y, 0.0f);

		XMMATRIX wvp = objMatrix * m_orthoMatrix;
		ConstantBuffer cb;
		cb.wvp = XMMatrixTranspose(wvp);
		deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		PixelBufferType pb;
		pb.color = color;
		deviceContext->UpdateSubresource(m_pixelBuffer, 0, NULL, &pb, 0, 0);
		deviceContext->PSSetConstantBuffers(0, 1, &m_pixelBuffer);
		deviceContext->PSSetShaderResources(0, 1, &m_texture);
		return;
	}

	void TextShader::render(ID3D11DeviceContext* deviceContext, int numIndices, XMFLOAT4 color, XMFLOAT2 pos, XMMATRIX projectionMatrix)
	{
		setShaderParameters(deviceContext, color, pos, projectionMatrix);

		deviceContext->IASetInputLayout(m_layout);
		deviceContext->VSSetShader(m_vShader, NULL, 0);
		deviceContext->PSSetShader(m_pShader, NULL, 0);
		if (m_sampleState)
			deviceContext->PSSetSamplers(0, 1, &m_sampleState);
		deviceContext->DrawIndexed(numIndices, 0, 0);
		return;
	}

	void TextShader::initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
	{
		initVertexShaders(device, vShaderFile, "VS");
		initPixelShaders(device, pShaderFile, "PS");
		m_constantBuffer = Buffer::createConstantBuffer(device, sizeof(ConstantBuffer), false);
		m_pixelBuffer = Buffer::createConstantBuffer(device, sizeof(PixelBufferType), false);

		return;
	}

	////////////////////////////////////   Light Shader   ///////////////////////////////////////////////

	void LightShader::setShaderParameters
	(
		ID3D11DeviceContext* deviceContext,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture[],
		Light* light,
		XMFLOAT3 cameraPos
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

		D3D11_MAPPED_SUBRESOURCE mappedResC;
		CameraBufferType* p_dataC;

		hr = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResC);
		p_dataC = (CameraBufferType*)mappedResC.pData;
		p_dataC->position = cameraPos;
		deviceContext->Unmap(m_cameraBuffer, 0);
		bufferNum = 1;
		deviceContext->PSSetConstantBuffers(bufferNum, 1, &m_cameraBuffer);

		deviceContext->PSSetShaderResources(0, 1, &texture[0]);
		deviceContext->PSSetShaderResources(1, 1, &texture[1]);
		deviceContext->PSSetShaderResources(2, 1, &texture[2]);
		deviceContext->PSSetShaderResources(3, 1, &texture[3]);

		D3D11_MAPPED_SUBRESOURCE mappedResL;
		LightBufferType* p_dataL;

		hr = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResL);
		p_dataL = (LightBufferType*)mappedResL.pData;
		p_dataL->ambientColor = light->ambientColor;
		p_dataL->diffuseColor = light->diffuseColor;
		p_dataL->specularColor = light->specularColor;
		p_dataL->direction = light->direction;
		p_dataL->specularPower = light->specularPower;
		deviceContext->Unmap(m_lightBuffer, 0);
		bufferNum = 0;
		deviceContext->PSSetConstantBuffers(bufferNum, 1, &m_lightBuffer);

	}

	void LightShader::initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
	{
		initVertexShaders(device, vShaderFile, "VS");
		initPixelShaders(device, pShaderFile, "PS");
		m_matrixBuffer = Buffer::createConstantBuffer(device, sizeof(MatrixBufferType), true);
		m_lightBuffer = Buffer::createConstantBuffer(device, sizeof(LightBufferType), true);
		m_cameraBuffer = Buffer::createConstantBuffer(device, sizeof(CameraBufferType), true);

		return;
	}

	void LightShader::render
	(
		ID3D11DeviceContext* deviceContext,
		int numIndices,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture[],
		Light* light,
		XMFLOAT3 cameraPos
	)
	{
		setShaderParameters(deviceContext, worldMatrix,
			viewMatrix, projectionMatrix, texture, light, cameraPos);

		deviceContext->IASetInputLayout(m_layout);
		deviceContext->VSSetShader(m_vShader, NULL, 0);
		deviceContext->PSSetShader(m_pShader, NULL, 0);
		if (m_sampleState)
			deviceContext->PSSetSamplers(0, 1, &m_sampleState);
		deviceContext->DrawIndexed(numIndices, 0, 0);
		return;
	}

	////////////////////////////////////   Fog Shader   ///////////////////////////////////////////////

	void FogShader::setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX viewMatrix)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedRes;
		ConstantBuffer* p_data;
		unsigned int bufferNum = 0;

		viewMatrix = XMMatrixTranspose(viewMatrix);

		hr = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
		p_data = (ConstantBuffer*)mappedRes.pData;
		p_data->wvp = viewMatrix;
		deviceContext->Unmap(m_matrixBuffer, 0);
		deviceContext->VSSetConstantBuffers(bufferNum, 1, &m_matrixBuffer);

		return;
	}

	void FogShader::render(ID3D11DeviceContext* deviceContext, XMMATRIX viewMatrix)
	{
		setShaderParameters(deviceContext, viewMatrix);

		deviceContext->IASetInputLayout(m_layout);
		deviceContext->VSSetShader(m_vShader, NULL, 0);
		deviceContext->PSSetShader(m_pShader, NULL, 0);
		if (m_sampleState)
			deviceContext->PSSetSamplers(0, 1, &m_sampleState);
		//deviceContext->DrawIndexed(3, 0, 0);
		deviceContext->DrawAuto();
		return;
	}

	void FogShader::initShaders(ID3D11Device* device, const char* vShaderFile, const char* pShaderFile)
	{
		initVertexShaders(device, vShaderFile, "VS");
		initPixelShaders(device, pShaderFile, "PS");
		m_matrixBuffer = Buffer::createConstantBuffer(device, sizeof(MatrixBufferType), true);
		return;
	}

}