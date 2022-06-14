#pragma once

#define NOMINMAX
#include <d3d11.h>
#include <d3dx11.h>
#include "basicStructs.h"
#include "buffer.h"
#include "model.h"
#include "camera.h"

#include "font.h"

#include "shader.h"

#pragma comment(lib, "d3d11.lib")
#ifdef _DEBUG
#	pragma comment(lib, "d3dx11d.lib")
#else
#	pragma comment(lib, "d3dx11.lib")
#endif

namespace MyEngine
{
	// ���������� �������� ������
	struct ScreenVertexData
	{
		XMFLOAT3  pos;
		XMFLOAT2  tex;
	};
	// ����� �������. ���������, ������� � ������� ������ (� �������)
	const int NUM_BUFFERS = 4; // diffuse3 + ao, normal3 +, positions???, roughness + metallness

	// ����� ������������� � ��11. ����� ����� ������ ��� �������� ��� �����������
	class VideoDriverDX11
	{

	protected:
		HWND hwnd; // ������� ����
		D3D_DRIVER_TYPE m_driverType; // ��� ��������
		D3D_FEATURE_LEVEL m_featureLevel; // ������ 

		ID3D11Device* m_device; // ������
		ID3D11DeviceContext* m_deviceContext; // ��������
		IDXGISwapChain* m_swapChain; // ��������

		ID3D11Texture2D* m_renderTargetTextureArray[NUM_BUFFERS]; // �������� ������� (3 ������)
		ID3D11ShaderResourceView* m_shaderResourceViewArray[NUM_BUFFERS]; // 
		ID3D11RenderTargetView* m_renderTargetViewArray[NUM_BUFFERS]; // ��� ����� ������ ������ ������� (��������� � �������)
		
		ID3D11RenderTargetView* m_renderTargetView; // ������ �����

		ID3D11Texture2D* m_depthStencilBuffer; // �������� ������ �������
		ID3D11DepthStencilView* m_depthStencilView; // ����� �������

		ID3D11DepthStencilState* m_depthStencilState; 
		ID3D11DepthStencilState* m_depthDisabledStencilState;

		ID3D11RasterizerState* m_rasterizerState; // ��������� ������������� 

		D3D11_VIEWPORT m_viewport; // �������

		ID3D11Buffer* m_vScreen = nullptr; // ����� ������ ������
		ID3D11Buffer* m_iScreen = nullptr; // ����� �������� ������

		XMMATRIX m_matrixProjection; // ������� ��������
		XMMATRIX m_matrixOrtho; // �����������

		XMMATRIX m_baseWorldMatrix; // ������� ������� ���� (���������) ��� ������ �������� ������� �� �����
		XMMATRIX m_baseViewMatrix; // ������� ������� ���� (���������) ��� ������ �������� ������� �� �����

		Font* fontFPS; // ����� ��� ������ ���
		Font* fontControl; // ����� ��� ������ ���������

		// �������� ���������� � ����� ������ ��������
		bool createDevice();

		int m_widthScreen; // ������ ������
		int m_heightScreen; // ������ ������
		// ��������� ����� ���������� ������� (��� ������ ������)
		void turnOnAlphaBlending(); 
		// ���������� ����� ���������� ������� (��� ������ ������)
		void turnOffAlphaBlending();

		ID3D11BlendState* m_alphaEnableBlendingState; // ��������� � ���������� �����������
		ID3D11BlendState* m_alphaDisableBlendingState;// ��������� � ����������� �����������

		LightShader* m_lightShader; // ������ �����

	public:
		Text* textFPS; // �����: fps = ...
		Text* textControl; // ����� ��������� �� ����������
		Text* textTimeDay; // ����� ��������� �� ����������
		// ��������� ������� ���������
		void renderShader(Light* light, Camera* camera);
		// ����� ������ �� �����
		void renderText(Text* text, XMFLOAT4 color, XMFLOAT2 pos);
		// �������� �������???
		void createBuffers();
		// �������� ������
		ID3D11Device* getDevice()
		{
			return m_device;
		}
		// �������� ��������
		ID3D11DeviceContext* getDeviceContext()
		{
			return m_deviceContext;
		}
		// �������� ������� �������
		IDXGISwapChain* getSwapChain()
		{
			return m_swapChain;
		}
		// ����������� �� ��������� - ��� ��������
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
			m_rasterizerState = nullptr;
			m_renderTargetView = nullptr;
		}

		// ���������� ������ � �������� ���� ����������
		void setRenderTargetBuffers();
		// ���������� ������ ����� ����� ����������
		void setRenderTargetBackBuffer();
		// �������� ������
		void clearRenderTarget();
		// ���������
		void renderToScreen();
		// �������� z �����
		void turnZBufferOn();
		// �������� z �����
		void turnZBufferOff();
		// ������������ ��� ���������� ������ xm ��������
		void* operator new(size_t i)
		{
			return _aligned_malloc(i, 16);
		}
		// ������� ���� �����������
		void operator delete(void* p)
		{
			_aligned_free(p);
		}
		// ������ ����� (������ �������)
		void beginScene();
		// ����� ����� - ���� �������
		void endScene();
		// ����� ������������� ��������
		void init(HWND hwnd_);




	};
	
}