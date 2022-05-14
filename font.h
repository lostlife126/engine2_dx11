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
	// ��������� � ������� �������� ���������� ������ (���������� ����� � ��������)
	struct VertexFont
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};


	// ����� � ������� �������� �����
	class Font
	{
	public:
		// ����������� �������� ������ � �������� ����� ������ �� �� ����������
		Font(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_):
			device(device_),
			deviceContext(deviceContext_)
		{}
		// ������������� (����� ������ ������ � ������ ������ (��������))
		void init(int wid_screen, int hei_screen, int size);
		// ���������� ����� ������ � ������
		void draw(unsigned int ind, XMFLOAT4 color, XMFLOAT2 pos);
		// ��������� �������� ������ �� ������� ������
		void buildVertexArray(VertexFont* vertices, int numVert, const char* sentence);
		// �������� ������
		ID3D11Device* getDevice()
		{
			return device;
		}
		// �������� ��������
		ID3D11DeviceContext* getContext()
		{
			return deviceContext;
		}

	protected:
		// ��������� ��� �������� .fnt ������
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

		ID3D11Device* device; // ����������
		ID3D11DeviceContext* deviceContext; // ��������

		XMMATRIX m_identityMatrix; // ��������� ������� ��� �������
		XMMATRIX m_orthoMatrix; // ����������� ��� ������ � ������

		unsigned short m_width_tex; //  ������ �������� � �������
		unsigned short m_height_tex; //  ������ �������� � �������
		unsigned short m_width_screen; // ������ �������� �� ������� �������� �����
		unsigned short m_height_screen; // ������ �������� �� ������� �������� �����
		std::map<int, CharDesc> m_charDesc; // ��������� ������������ ������ ������� � ��� ��������� CharDesc
		TextShader* m_shader; // ������ ��� ������
		std::string filenameTex; // �������� ����� ��������
		int m_size; // ������ ������
		// ������� fnt ���� filename � ������� ������ CharDesc ��������� 
		void parse(const char* filename);

	};


	// ����� ��������������� ����� ������� ��������� �� �����
	class Text
	{
	public:
		// � ������������ ������ ����� ������� ����� �������������� ��� ������
		Text(Font* font) :pFont(font)
		{}
		// �������������. str - ������ ������� �������, numChars - ���� ����� ��������, dynamic - ����� �� ������ ������?
		void init(const char* str, int numChars, bool m_dynamic_ = false);
		// ������� ����� �� ����� � color ������ � � ��������� pos
		void render(XMFLOAT4 color, XMFLOAT2 pos);
		// �������� ����� (���� ������������)
		void setText(const char* text);

	protected:
		// ������ ��������� � ��������� ������ � ������
		void renderBuffers();

		Font* pFont; // �����
		ID3D11Buffer* m_vertexBuffer; // ��������� �����
		ID3D11Buffer* m_indexBuffer;// ��������� �����
		int m_strlen; // ������� ������ ������
		int max_charsize;// ������������ ������ ������
		int m_numInd; // ����� �������� (�� ������ ������ �� 6 �������� (��� ������������))
		int m_numVert; //  ����� ��������� (�����) ������ (�� ������ ������ �� 4 ��������)
		bool m_dynamic; // ������������ �� ������?
	};
	

}