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
	// координаты текстуры экрана
	struct ScreenVertexData
	{
		XMFLOAT3  pos;
		XMFLOAT2  tex;
	};
	// число буферов. диффузный, нормаль и амбиент оклюжн (в будущем)
	const int NUM_BUFFERS = 4; // diffuse3 + ao, normal3 +, positions???, roughness + metallness

	// класс видеодрайвера в дх11. здесь будут сидеть все действия над рендерингом
	class VideoDriverDX11
	{

	protected:
		HWND hwnd; // хэндлер окна
		D3D_DRIVER_TYPE m_driverType; // тип драйвера
		D3D_FEATURE_LEVEL m_featureLevel; // версия 

		ID3D11Device* m_device; // девайс
		ID3D11DeviceContext* m_deviceContext; // контекст
		IDXGISwapChain* m_swapChain; // свопчейн

		ID3D11Texture2D* m_renderTargetTextureArray[NUM_BUFFERS]; // текстуры буферов (3 буфера)
		ID3D11ShaderResourceView* m_shaderResourceViewArray[NUM_BUFFERS]; // 
		ID3D11RenderTargetView* m_renderTargetViewArray[NUM_BUFFERS]; // тут лежат буферы разных текстур (диффузная и нормали)
		
		ID3D11RenderTargetView* m_renderTargetView; // задний буфер

		ID3D11Texture2D* m_depthStencilBuffer; // текстура буфера глубины
		ID3D11DepthStencilView* m_depthStencilView; // буфер глубины

		ID3D11DepthStencilState* m_depthStencilState; 
		ID3D11DepthStencilState* m_depthDisabledStencilState;

		ID3D11RasterizerState* m_rasterizerState; // состояние растеризатора 

		D3D11_VIEWPORT m_viewport; // вьюпорт

		ID3D11Buffer* m_vScreen = nullptr; // буфер вершин экрана
		ID3D11Buffer* m_iScreen = nullptr; // буфер индексов экрана

		XMMATRIX m_matrixProjection; // матрица проекции
		XMMATRIX m_matrixOrtho; // ортоматрица

		XMMATRIX m_baseWorldMatrix; // базовая матрица мира (единичная) для вывода текстуры буферов на экран
		XMMATRIX m_baseViewMatrix; // базовая матрица вида (единичная) для вывода текстуры буферов на экран

		Font* fontFPS; // шрифт для вывода фпс
		Font* fontControl; // шрифт для вывода подсказки

		// создание устройства и всего самого базового
		bool createDevice();

		int m_widthScreen; // ширина экрана
		int m_heightScreen; // высота экрана
		// включение альфа смешивания текстур (для вывода текста)
		void turnOnAlphaBlending(); 
		// выключение альфа смешивания текстур (для вывода текста)
		void turnOffAlphaBlending();

		ID3D11BlendState* m_alphaEnableBlendingState; // состояние с включенным смешиванием
		ID3D11BlendState* m_alphaDisableBlendingState;// состояние с выключенным смешиванием

		LightShader* m_lightShader; // шейдер света

	public:
		Text* textFPS; // текст: fps = ...
		Text* textControl; // текст подсказки по управлению
		Text* textTimeDay; // текст подсказки по управлению
		// рендеринг шейдера освещения
		void renderShader(Light* light, Camera* camera);
		// вывод текста на экран
		void renderText(Text* text, XMFLOAT4 color, XMFLOAT2 pos);
		// создание буферов???
		void createBuffers();
		// получить девайс
		ID3D11Device* getDevice()
		{
			return m_device;
		}
		// получить контекст
		ID3D11DeviceContext* getDeviceContext()
		{
			return m_deviceContext;
		}
		// получить цепочку буферов
		IDXGISwapChain* getSwapChain()
		{
			return m_swapChain;
		}
		// конструктор по умолчанию - все зануляем
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

		// установить буферы в качестве цели рендеринга
		void setRenderTargetBuffers();
		// установить задний буфер целью рендеринга
		void setRenderTargetBackBuffer();
		// очистить буферы
		void clearRenderTarget();
		// рендерить
		void renderToScreen();
		// включить z буфер
		void turnZBufferOn();
		// включить z буфер
		void turnZBufferOff();
		// выравнивание для нормальной работы xm структур
		void* operator new(size_t i)
		{
			return _aligned_malloc(i, 16);
		}
		// удаляем тоже выровненные
		void operator delete(void* p)
		{
			_aligned_free(p);
		}
		// начало сцены (чистка буферов)
		void beginScene();
		// конце сцены - своп буферов
		void endScene();
		// общая инициализация драйвера
		void init(HWND hwnd_);




	};
	
}