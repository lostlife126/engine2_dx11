#include "videoDriverDX11.h"

#include <d3dcompiler.h>
#include <xnamath.h>
namespace MyEngine
{

	void VideoDriverDX11::init(HWND hwnd_)
	{
		hwnd = hwnd_;
		createDevice();


	}

	bool VideoDriverDX11::createDevice()
	{
		HRESULT hr = S_OK;
		RECT rc;
		GetClientRect(hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;
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
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
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
				&m_pSwapChain,
				&m_pDevice,
				&m_featureLevel,
				&m_pContextImmediate
			);
			if (SUCCEEDED(hr))
			{
				break;
			}
		}
		if (FAILED(hr))
		{
			Log::Get()->Error("Error creation DX11 device.");
		}

		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
		if (FAILED(hr))
		{
			Log::Get()->Error("Error geting SwapChain buffer.");
		}

		hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		if (FAILED(hr))
		{
			Log::Get()->Error("Error creation target buffer.");
		}

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
		if (FAILED(hr))
		{
			Log::Get()->Error("Error creation texture depth.");
		}
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
		if (FAILED(hr))
		{
			Log::Get()->Error("Error creation stencil view.");
		}

		m_pContextImmediate->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		D3D11_VIEWPORT vp;
		vp.Width = float(width);
		vp.Height = float(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pContextImmediate->RSSetViewports(1, &vp);

		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		m_pDevice->CreateRasterizerState(&desc, &m_pRasterizerState_wire);
		desc.FillMode = D3D11_FILL_SOLID;
		m_pDevice->CreateRasterizerState(&desc, &m_pRasterizerState_solid);

		return true;
	}

	void VideoDriverDX11::beginScene()
	{
		float clearColor[4] = { 0.0f, 0.125f, 0.25f, 0.0f };
		m_pContextImmediate->ClearRenderTargetView(m_pRenderTargetView, clearColor);
		m_pContextImmediate->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		if (stateNow == 1)
		{
			m_pContextImmediate->RSSetState(m_pRasterizerState_wire);
		}
		else
		{
			m_pContextImmediate->RSSetState(m_pRasterizerState_solid);
		}
	}

	void VideoDriverDX11::endScene()
	{ 
		m_pSwapChain->Present(0, 0); 
	};

}