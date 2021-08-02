#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <fstream>
#include <sstream>
#include <map>
#include"log.h"
#include "shader.h"

namespace MyEngine
{

	struct VertexFont
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

	class Font
	{
	public:

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

		struct ConstantBuffer
		{
			XMMATRIX wvp;
		};

		struct PixelBufferType
		{
			XMFLOAT4 color;
		};

		unsigned short m_width_tex;
		unsigned short m_height_tex;
		unsigned short m_width_screen = 640;
		unsigned short m_height_screen = 480;
		std::map<int, CharDesc> m_charDesc;
		Shader* m_shader;

		ID3D11Buffer* m_constantBuffer;
		ID3D11Buffer* m_pixelBuffer;

		Font() {}

		void parse(const char* filename)
		{
			std::ifstream fin;
			fin.open(filename);
			if (fin.fail())
				return;
			std::string line, key, value, read;
			size_t i;

			while (!fin.eof())
			{
				std::stringstream lineStream;
				std::getline(fin, line);
				lineStream << line;
				lineStream >> read;
				if (read == "common")
				{
					while (!lineStream.eof())
					{
						std::stringstream converter;
						lineStream >> read;
						i = read.find('=');
						key = read.substr(0, i);
						value = read.substr(i + 1);
						converter << value;
						if (key == "scaleW")
							converter >> m_width_tex;
						else if (key == "scaleH")
							converter >> m_height_tex;
					}
				}
				else if (read == "page")
				{
					while (!lineStream.eof())
					{
						std::stringstream converter;
						lineStream >> read;
						i = read.find('=');
						key = read.substr(0, i);
						value = read.substr(i + 1);

						std::string str;
						converter << value;
						if (key == "file")
						{
							converter >> str;

						}
					}
				}
				else if (read == "char")
				{
					unsigned short charID = 0;
					CharDesc ch;
					while (!lineStream.eof())
					{
						std::stringstream converter;
						lineStream >> read;
						i = read.find('=');
						key = read.substr(0, i);
						value = read.substr(i + 1);

						converter << value;
						if (key == "id")
							converter >> charID;
						else if (key == "x")
							converter >> ch.srcX;
						else if (key == "y")
							converter >> ch.srcY;
						else if (key == "width")
							converter >> ch.srcW;
						else if (key == "height")
							converter >> ch.srcH;
						else if (key == "xoffset")
							converter >> ch.xOff;
						else if (key == "yoffset")
							converter >> ch.yOff;
						else if (key == "xadvance")
							converter >> ch.xAdv;
					}
					m_charDesc.insert(std::pair<int, CharDesc>(charID, ch));
				}
			}
			fin.close();
		}

		void init(ID3D11Device* device, int wid_screen, int hei_screen)
		{
			m_width_screen = wid_screen;
			m_height_screen = hei_screen;
			parse("font.fnt");

			m_shader = new Shader;
			m_shader->loadTexture(device, "font_0.png");

			m_shader->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
			m_shader->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
			m_shader->initShaders(device, "text_vs.fx", "text_ps.fx");

			m_constantBuffer = Buffer::createConstantBuffer(device, sizeof(ConstantBuffer), false);
			m_pixelBuffer = Buffer::createConstantBuffer(device, sizeof(PixelBufferType), false);

			return;
		}

		void buildVertexArray(VertexFont* vertices, int numVert, const char* sentence)
		{
			int lenStr = strlen(sentence);
			if (lenStr * 4 > numVert)
				lenStr = numVert / 4;
			float drawX = (float)-m_width_screen / 2;
			float drawY = (float)m_height_screen / 2;
			int ind = 0;
			for (int i = 0; i < lenStr; i++)
			{
				float charX = m_charDesc[sentence[i]].srcX;
				float charY = m_charDesc[sentence[i]].srcY;
				float width = m_charDesc[sentence[i]].srcW;
				float height = m_charDesc[sentence[i]].srcH;
				float xOff = m_charDesc[sentence[i]].xOff;
				float yOff = m_charDesc[sentence[i]].yOff;

				float left = drawX + xOff;
				float right = left + width;
				float top = drawY - yOff;
				float bottom = top - height;
				float left_tex = charX / m_width_tex;
				float right_tex = (charX + width) / m_width_tex;
				float top_tex = charY / m_height_tex;
				float bottom_tex = (charY + height) / m_height_tex;

				vertices[ind].pos = XMFLOAT3(left, top, 0.0f);
				vertices[ind].tex = XMFLOAT2(left_tex, top_tex);
				ind++;
				vertices[ind].pos = XMFLOAT3(right, bottom, 0.0f);
				vertices[ind].tex = XMFLOAT2(right_tex, bottom_tex);
				ind++;
				vertices[ind].pos = XMFLOAT3(left, bottom, 0.0f);
				vertices[ind].tex = XMFLOAT2(left_tex, bottom_tex);
				ind++;
				vertices[ind].pos = XMFLOAT3(right, top, 0.0f);
				vertices[ind].tex = XMFLOAT2(right_tex, top_tex);
				ind++;
				drawX += m_charDesc[sentence[i]].xAdv;
			}
		}

		void draw(ID3D11DeviceContext* deviceContext, unsigned int ind, float r, float g, float b, float x, float y)
		{
			setShaderParameters(deviceContext, r, g, b, x, y);
			m_shader->draw(deviceContext);
			deviceContext->DrawIndexed(ind, 0, 0);
		}

		void setShaderParameters(ID3D11DeviceContext* deviceContext, float r, float g, float b, float x, float y)
		{
			XMMATRIX objMatrix = XMMatrixTranslation(x, -y, 0.0f);

			XMMATRIX ortho = XMMatrixOrthographicLH(800.0f, 600.0f, 0.0f, 1000.0f);
			XMMATRIX wvp = objMatrix * ortho;////////////////////////////
			ConstantBuffer cb;
			cb.wvp = XMMatrixTranspose(wvp);
			deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);
			deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
			PixelBufferType pb;
			pb.color = XMFLOAT4(r, g, b, 1.0f);
			deviceContext->UpdateSubresource(m_pixelBuffer, 0, NULL, &pb, 0, 0);
			deviceContext->PSSetConstantBuffers(0, 1, &m_pixelBuffer);
		}

	};

	class Text
	{
	public:

		Text(Font* font):m_font(font)
		{}

		void init(ID3D11Device* device, const char* str, int numChars, bool m_dynamic_ = false)
		{
			m_dynamic = m_dynamic_;
			m_strlen = strlen(str);
			max_charsize = numChars;
			if (m_strlen > max_charsize)
			{
				Log::Get()->Error("Max length of line was exceeded! Line was truncated!");
				m_strlen = max_charsize;
			}
			m_numVert = 4 * max_charsize;
			VertexFont* vertex = new VertexFont[m_numVert];

			m_font->buildVertexArray(vertex, m_numVert, str);
			m_numInd = 6 * max_charsize;
			unsigned long* indices = new unsigned long[m_numInd];
			for (int i = 0; i < max_charsize; i++)
			{
				indices[i * 6]     = i * 4;
				indices[i * 6 + 1] = i * 4 + 1;
				indices[i * 6 + 2] = i * 4 + 2;
				indices[i * 6 + 3] = i * 4;
				indices[i * 6 + 4] = i * 4 + 3;
				indices[i * 6 + 5] = i * 4 + 1;
			}

			m_vertexBuffer = Buffer::createVertexBuffer(device, sizeof(VertexFont) * m_numVert, vertex, m_dynamic);
			m_indexBuffer = Buffer::createIndexBuffer(device, sizeof(unsigned long) * m_numInd, indices, m_dynamic);
			
			delete[] vertex;
			delete[] indices;
		}

		void render(ID3D11DeviceContext* deviceContext, float r, float g, float b, float x, float y)
		{
			renderBuffers(deviceContext);
			m_font->draw(deviceContext, m_strlen * 6, r,g,b,x,y);
		}

		void renderBuffers(ID3D11DeviceContext* deviceContext)
		{
			unsigned int stride = sizeof(VertexFont);
			unsigned int offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void setText(ID3D11DeviceContext* deviceContext, const char* text)
		{
			if (m_dynamic)
			{
				m_strlen = strlen(text);
				if (m_strlen > max_charsize)
				{
					m_strlen = max_charsize;
				}
				//m_numInd = 6 * m_strlen;
				D3D11_MAPPED_SUBRESOURCE mr;
				HRESULT hr = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
				VertexFont* vertPtr = (VertexFont*)mr.pData;
				m_font->buildVertexArray(vertPtr, m_numVert, text);
				deviceContext->Unmap(m_vertexBuffer, 0);
			}
			else
			{
				Log::Get()->Error("Attempt to change static text!");
			}
		}

		Font* m_font;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		int m_strlen;
		int max_charsize;
		int m_numInd;
		int m_numVert;
		bool m_dynamic;

	};

}