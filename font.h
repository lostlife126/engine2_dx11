#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <fstream>
#include <sstream>
#include <map>

#include "shader.h"

namespace MyEngine
{
	// структура в которой хранятся координаты шрифта (координата точки и текстуры)
	struct VertexFont
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};


	// класс в котором хранится шрифт
	class Font
	{
	public:
		// конструктор получает девайс и контекст чтобы больше их не спрашивать
		Font(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_):
			device(device_),
			deviceContext(deviceContext_)
		{}
		// инициализация (нужен размер экрана и размер шрифта (условный))
		void init(int wid_screen, int hei_screen, int size);
		// нарисовать текст данный в шейдер
		void draw(unsigned int ind, XMFLOAT4 color, XMFLOAT2 pos);
		// построить текстуру текста по данному тексту
		void buildVertexArray(VertexFont* vertices, int numVert, const char* sentence);
		// получить девайс
		ID3D11Device* getDevice()
		{
			return device;
		}
		// получить контекст
		ID3D11DeviceContext* getContext()
		{
			return deviceContext;
		}

	protected:
		// структура для парсинга .fnt файлов
		struct CharDesc
		{
			CharDesc(): srcX(0), srcY(0), srcW(0), srcH(0), xOff(0), yOff(0), xAdv(0) {}
			short srcX;
			short srcY;
			short srcW;
			short srcH;
			short xOff;
			short yOff;
			short xAdv;
		};

		ID3D11Device* device; // устройство
		ID3D11DeviceContext* deviceContext; // контекст

		XMMATRIX m_identityMatrix; // единичная матрица для шейдера
		XMMATRIX m_orthoMatrix; // ортоматрица для вывода в шейдер

		unsigned short m_width_tex; //  ширина текстуры с шрифтом
		unsigned short m_height_tex; //  высота текстуры с шрифтом
		unsigned short m_width_screen; // ширина текстуры на которую рисуется текст
		unsigned short m_height_screen; // высота текстуры на которую рисуется текст
		std::map<int, CharDesc> m_charDesc; // контейнер соответствия номера символа и его структуры CharDesc
		TextShader* m_shader; // шейдер для шрифта
		std::string filenameTex; // название файла текстуры
		int m_size; // размер шрифта
		// парсить fnt файл filename и достать оттуда CharDesc структуры 
		void parse(const char* filename);

	};


	// класс инкапсулирующий текст который выводится на экран
	class Text
	{
	public:
		// в конструкторе подаем шрифт который будет использоваться для вывода
		Text(Font* font) :pFont(font)
		{}
		// инициализация. str - строка которую выводим, numChars - макс число символов, dynamic - можем ли менять строку?
		void init(const char* str, int numChars, bool m_dynamic_ = false);
		// вывести текст на экран с color цветом и в положении pos
		void render(XMFLOAT4 color, XMFLOAT2 pos);
		// изменить текст (если динамический)
		void setText(const char* text);

	protected:
		// задать векторный и индексный буферы в шейдер
		void renderBuffers();

		Font* pFont; // шрифт
		ID3D11Buffer* m_vertexBuffer; // векторный буфер
		ID3D11Buffer* m_indexBuffer;// индексный буфер
		int m_strlen; // текущий размер строки
		int max_charsize;// максимальный размер строки
		int m_numInd; // число индексов (на каждый символ по 6 индексов (два треугольника))
		int m_numVert; //  число вертексов (точек) текста (на каждый символ по 4 вертекса)
		bool m_dynamic; // динамическая ли строка?
	};
	

}