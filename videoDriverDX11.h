#pragma once

#define NOMINMAX
#include <d3d11.h>
#include <d3dx11.h>
#include "basicStructs.h"
#include "buffer.h"
#include "model.h"
#include "camera.h"
#include "log.h"
#include <xnamath.h>
#include<d3dx10math.h>
#include "shader.h"

#pragma comment(lib, "d3d11.lib")
#ifdef _DEBUG
#	pragma comment(lib, "d3dx11d.lib")
#else
#	pragma comment(lib, "d3dx11.lib")
#endif

namespace MyEngine
{
	class VideoDriverDX11
	{
	public:
		HWND hwnd;
		D3D_DRIVER_TYPE m_driverType;
		D3D_FEATURE_LEVEL m_featureLevel;

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContextImmediate;
		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;

		ID3D11Texture2D* m_pDepthStencil;
		ID3D11DepthStencilView* m_pDepthStencilView;

		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11DepthStencilState* m_pDepthDisabledStencilState;

		ID3D11RasterizerState* m_pRasterizerState_wire;
		ID3D11RasterizerState* m_pRasterizerState_solid;
		int stateNow;

		// camera
		XMMATRIX m_View;
		XMMATRIX m_Projection;
		XMMATRIX m_World;

		int width_screen;
		int height_screen;

		VideoDriverDX11()
		{
			m_driverType = D3D_DRIVER_TYPE_NULL;
			m_featureLevel = D3D_FEATURE_LEVEL_11_0;
			m_pDevice = nullptr;
			m_pContextImmediate = nullptr;
			m_pSwapChain = nullptr;
			m_pRenderTargetView = nullptr;
			m_pDepthStencil = nullptr;
			m_pDepthStencilView = nullptr;
			m_pDepthStencilState = nullptr;
			m_pDepthDisabledStencilState = nullptr;
			m_pRasterizerState_wire = nullptr;
			m_pRasterizerState_solid = nullptr;
			stateNow = 0;
		}

		void changeState()
		{
			if (stateNow == 0)
			{
				stateNow = 1;
			}
			else
			{
				stateNow = 0;
			}
		}

		void* operator new(size_t i)
		{
			return _aligned_malloc(i, 16);
		}

		void operator delete(void* p)
		{
			_aligned_free(p);
		}
		void beginScene();
		void endScene();

		void init(HWND hwnd_);

		bool createDevice();

	};
	
}