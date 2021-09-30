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

	struct ScreenVertexData
	{
		XMFLOAT3  pos;
		XMFLOAT2  tex;
	};

	const int NUM_BUFFERS = 2;

	class VideoDriverDX11
	{

	private:
		HWND hwnd;
		D3D_DRIVER_TYPE m_driverType;
		D3D_FEATURE_LEVEL m_featureLevel;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		IDXGISwapChain* m_swapChain;

		ID3D11Texture2D* m_renderTargetTextureArray[NUM_BUFFERS];
		ID3D11ShaderResourceView* m_shaderResourceViewArray[NUM_BUFFERS];
		ID3D11RenderTargetView* m_renderTargetViewArray[NUM_BUFFERS]; // тут лежат буферы разных текстур (диффузная и нормали)
		
		ID3D11RenderTargetView* m_renderTargetView; // задний буфер

		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilView* m_depthStencilView;

		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilState* m_depthDisabledStencilState;

		ID3D11RasterizerState* m_rasterizerSolidState; // состояние растеризатора с сплошной заливкой
		ID3D11RasterizerState* m_rasterizerWireState; // состояние растеризатора с проволочной заливкой

		D3D11_VIEWPORT m_viewport; // вьюпорт

		ID3D11Buffer* m_vScreen = nullptr;
		ID3D11Buffer* m_iScreen = nullptr;

		Shader* m_lightShader;

		XMMATRIX m_matrixProjection;
		XMMATRIX m_matrixOrtho;

		bool createDevice();
		bool solidState;

		int m_widthScreen;
		int m_heightScreen;

	public:

		void createBuffers();

		ID3D11Device* getDevice()
		{
			return m_device;
		}
		ID3D11DeviceContext* getDeviceContext()
		{
			return m_deviceContext;
		}
		IDXGISwapChain* getSwapChain()
		{
			return m_swapChain;
		}

		VideoDriverDX11()
		{
			m_driverType = D3D_DRIVER_TYPE_NULL;
			m_featureLevel = D3D_FEATURE_LEVEL_11_0;
			m_device = nullptr;
			m_deviceContext = nullptr;
			m_swapChain = nullptr;
			m_depthStencilBuffer = nullptr;
			m_depthStencilView = nullptr;
			m_depthStencilState = nullptr;
			m_depthDisabledStencilState = nullptr;
			m_rasterizerWireState = nullptr;
			m_rasterizerSolidState = nullptr;
			m_renderTargetView = nullptr;
			solidState = true;
		}

		void setRenderTargets();
		void clearRenderTargets();
		void resetViewport();

		void renderOrtho();

		void setBackBufferTarget();

		void turnZBufferOn();
		void turnZBufferOff();

		void changeState();

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





	};
	
}