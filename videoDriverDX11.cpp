#include "videoDriverDX11.h"

#include <d3dcompiler.h>
#include <xnamath.h>
#include "log.h"
namespace MyEngine
{
	void VideoDriverDX11::init(HWND hwnd_)
	{
		hwnd = hwnd_;
		createDevice();

		float left = (float)((m_widthScreen / 2) * -1);
		float right = left + (float)m_widthScreen;
		float top = (float)(m_heightScreen / 2);
		float bottom = top - (float)m_heightScreen;
		m_viewport.Width = float(m_widthScreen);
		m_viewport.Height = float(m_heightScreen);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;

		m_baseWorldMatrix = XMMatrixIdentity();
		m_baseViewMatrix = XMMatrixIdentity();

		createBuffers();

		std::vector<ScreenVertexData> vScreen(4);
		std::vector<DWORD> iScreen(6);
		vScreen[0].pos = XMFLOAT3(left, top, 0.0f);
		vScreen[0].tex = XMFLOAT2(0.0f, 0.0f);
		vScreen[1].pos = XMFLOAT3(right, top, 0.0f);
		vScreen[1].tex = XMFLOAT2(1.0f, 0.0f);
		vScreen[2].pos = XMFLOAT3(left, bottom, 0.0f);
		vScreen[2].tex = XMFLOAT2(0.0f, 1.0f);
		vScreen[3].pos = XMFLOAT3(right, bottom, 0.0f);
		vScreen[3].tex = XMFLOAT2(1.0f, 1.0f);
		iScreen[0] = 0;
		iScreen[1] = 1;
		iScreen[2] = 2;
		iScreen[3] = 2;
		iScreen[4] = 1;
		iScreen[5] = 3;
		m_vScreen = Buffer::createVertexBuffer(m_device, sizeof(ScreenVertexData) * 4, &(vScreen[0]), false);
		m_iScreen = Buffer::createIndexBuffer(m_device, sizeof(DWORD) * 6, &(iScreen[0]), false);

		m_matrixProjection = XMMatrixPerspectiveFovLH(0.7, 1.3333, 0.1f, 1000.0f);
		m_matrixOrtho = XMMatrixOrthographicLH(640, 480, 0.1f, 1000.0f);

		m_lightShader = new LightShader;
		m_lightShader->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		m_lightShader->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		m_lightShader->initShaders(m_device, "light_vs.fx", "light_ps.fx");
		m_lightShader->setSampleState(m_device);

		fontFPS = new Font(m_device, m_deviceContext);
		fontFPS->init(m_widthScreen, m_heightScreen, 10);
		textFPS = new Text(fontFPS);
		textFPS->init("fps = ", 12, true);
		fontControl = new Font(m_device, m_deviceContext);
		fontControl->init(m_widthScreen, m_heightScreen, 7);
		textControl = new Text(fontControl);
		textControl->init("w - forward, s - back, a - left, d - right, q - lock mouse, esc - exit", 72, false);

		D3D11_BLEND_DESC blendStateDesc;
		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

		// Create an alpha enabled blend state description.
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		// Create the blend state using the description.
		HRESULT result = m_device->CreateBlendState(&blendStateDesc, &m_alphaEnableBlendingState);
		if (FAILED(result))
		{
			return;
		}

		// Modify the description to create an alpha disabled blend state description.
		blendStateDesc.RenderTarget[0].BlendEnable = FALSE;

		// Create the blend state using the description.
		result = m_device->CreateBlendState(&blendStateDesc, &m_alphaDisableBlendingState);
		if (FAILED(result))
		{
			return;
		}
	}

	void VideoDriverDX11::createBuffers()
	{
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = m_viewport.Width;
		texDesc.Height = m_viewport.Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		HRESULT hr;
		for (int i = 0; i < NUM_BUFFERS; i++)
		{
			hr = m_device->CreateTexture2D(&texDesc, NULL, &m_renderTargetTextureArray[i]);
			if(FAILED(hr))
				Log::Get()->Error("Error creation texture in driver.");
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		for (int i = 0; i < NUM_BUFFERS; i++)
		{
			hr = m_device->CreateRenderTargetView(m_renderTargetTextureArray[i], &rtvDesc, &m_renderTargetViewArray[i]);
			if (FAILED(hr))
				Log::Get()->Error("Error creation render target view in driver.");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		for (int i = 0; i < NUM_BUFFERS; i++)
		{
			hr = m_device->CreateShaderResourceView(m_renderTargetTextureArray[i], &srvDesc, &m_shaderResourceViewArray[i]);
			if (FAILED(hr))
				Log::Get()->Error("Error creation shader resource view in driver.");
		}

		D3D11_TEXTURE2D_DESC depthBuffDesc;
		ZeroMemory(&depthBuffDesc, sizeof(depthBuffDesc));
		depthBuffDesc.Width = m_viewport.Width;
		depthBuffDesc.Height = m_viewport.Height;
		depthBuffDesc.MipLevels = 1;
		depthBuffDesc.ArraySize = 1;
		depthBuffDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBuffDesc.SampleDesc.Count = 1;
		depthBuffDesc.SampleDesc.Quality = 0;
		depthBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBuffDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBuffDesc.CPUAccessFlags = 0;
		depthBuffDesc.MiscFlags = 0;
		hr = m_device->CreateTexture2D(&depthBuffDesc, NULL, &m_depthStencilBuffer);
		if (FAILED(hr))
			Log::Get()->Error("Error creation texture depth stencil buffer in driver.");

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &descDSV, &m_depthStencilView);
		if (FAILED(hr))
			Log::Get()->Error("Error creation depth stencil view.");
	}

	bool VideoDriverDX11::createDevice()
	{
		HRESULT hr = S_OK;
		RECT rc;
		GetClientRect(hwnd, &rc);
	    m_widthScreen = rc.right - rc.left;
		m_heightScreen = rc.bottom - rc.top;
		UINT createDeviceFlags = 0;

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = m_widthScreen;
		sd.BufferDesc.Height = m_heightScreen;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;

		for (UINT i = 0; i < numDriverTypes; i++)
		{
			m_driverType = driverTypes[i];
			hr = D3D11CreateDeviceAndSwapChain
			(
				NULL,
				m_driverType,
				NULL,
				createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&sd,
				&m_swapChain,
				&m_device,
				&m_featureLevel,
				&m_deviceContext
			);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			Log::Get()->Error("Error creation DX11 device.");

		// берем задний буфер из swapChain и прикрепляем его к renderTargetView для дальнейшей работы
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
		if (FAILED(hr))
			Log::Get()->Error("Error geting SwapChain buffer.");

		hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
		if (FAILED(hr))
			Log::Get()->Error("Error creation render target buffer.");

		pBackBuffer->Release(); pBackBuffer = nullptr;

		D3D11_DEPTH_STENCIL_DESC descDS;
		ZeroMemory(&descDS, sizeof(descDS));
		descDS.DepthEnable = true;
		descDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDS.DepthFunc = D3D11_COMPARISON_LESS;
		descDS.StencilEnable = true;
		descDS.StencilReadMask = 0xff;
		descDS.StencilWriteMask = 0xff;
		descDS.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDS.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDS.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDS.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDS.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDS.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDS.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDS.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = m_device->CreateDepthStencilState(&descDS, &m_depthStencilState);
		if (FAILED(hr))
			Log::Get()->Error("Error creation depth stencil state.");
		descDS.DepthEnable = false;
		hr = m_device->CreateDepthStencilState(&descDS, &m_depthDisabledStencilState);
		if (FAILED(hr))
			Log::Get()->Error("Error creation depth stencil state.");
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;
		m_device->CreateRasterizerState(&desc, &m_rasterizerState);
		m_deviceContext->RSSetState(m_rasterizerState);

		return true;
	}

	void VideoDriverDX11::setRenderTargetBuffers()
	{
		m_deviceContext->OMSetRenderTargets(NUM_BUFFERS, m_renderTargetViewArray, m_depthStencilView);
		m_deviceContext->RSSetViewports(1, &m_viewport);
		return;
	}

	void VideoDriverDX11::setRenderTargetBackBuffer()
	{
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		m_deviceContext->RSSetViewports(1, &m_viewport);
		return;
	}

	void VideoDriverDX11::clearRenderTarget()
	{
		float colorClear[4] = { 0.0, 0.0, 0.0, 1.0 };
		for (int i = 0; i < NUM_BUFFERS; i++)
		{
			m_deviceContext->ClearRenderTargetView(m_renderTargetViewArray[i], colorClear);
		}
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
		
	}

	void VideoDriverDX11::turnZBufferOn()
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
		return;
	}

	void VideoDriverDX11::turnZBufferOff()
	{
		m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
		return;
	}

	void VideoDriverDX11::renderToScreen()
	{
		unsigned int stride = sizeof(ScreenVertexData);
		unsigned int offset = 0;

		m_deviceContext->IASetVertexBuffers(0, 1, &m_vScreen, &stride, &offset);
		m_deviceContext->IASetIndexBuffer(m_iScreen, DXGI_FORMAT_R32_UINT, 0);
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void VideoDriverDX11::beginScene()
	{
		float clearColor[4] = { 0.0f, 0.125f, 0.25f, 0.0f };
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void VideoDriverDX11::endScene()
	{ 
		m_swapChain->Present(0, 0); 
	};

	void VideoDriverDX11::renderShader(Light* light)
	{
		m_lightShader->render(m_deviceContext, 6, m_baseWorldMatrix, m_baseViewMatrix, m_matrixOrtho, m_shaderResourceViewArray[0], m_shaderResourceViewArray[1], light);
	}

	void VideoDriverDX11::renderText(Text* text, XMFLOAT4 color, XMFLOAT2 pos)
	{
		turnOnAlphaBlending();
		text->render(color, pos);
		turnOffAlphaBlending();
	}

	void VideoDriverDX11::turnOnAlphaBlending()
	{
		float blendFactor[4] = { 0.0, 0.0, 0.0, 0.0 };
		m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
	}

	void VideoDriverDX11::turnOffAlphaBlending()
	{
		float blendFactor[4] = { 0.0, 0.0, 0.0, 0.0 };
		m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
	}

}