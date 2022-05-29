#include "font.h"
#include"log.h"

namespace MyEngine
{
	void Font::parse(const char* filename)
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
						converter >> filenameTex;
						filenameTex = filenameTex.substr(1, filenameTex.size() - 2);
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

	void Font::init(int wid_screen, int hei_screen, int size)
	{
		m_size = size;
		m_identityMatrix = XMMatrixIdentity();
		m_orthoMatrix = XMMatrixOrthographicLH(wid_screen, hei_screen, 0.0f, 1000.0f);
		m_width_screen = wid_screen ;
		m_height_screen = hei_screen ;
		parse("font.fnt");

		m_shader = new TextShader;
		m_shader->loadTextures(device, filenameTex.c_str());

		m_shader->addInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		m_shader->addInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		m_shader->initShaders(device, "text_vs.fx", "text_ps.fx");

		return;
	}

	// построить текстуру текста по данному тексту
	void Font::buildVertexArray(VertexFont* vertices, int numVert, const char* sentence)
	{
		float sizeLetter = m_size * 0.05;
		int lenStr = strlen(sentence);
		if (lenStr * 4 > numVert)
			lenStr = numVert / 4;
		float drawX = (float)-m_width_screen / 2;
		float drawY = (float)m_height_screen / 2;
		int ind = 0;
		for (int i = 0; i < lenStr; i++)
		{
			float charX = m_charDesc[sentence[i]].srcX; // координата x символа на текстуре
			float charY = m_charDesc[sentence[i]].srcY; // координата y символа на текстуре
			float width = m_charDesc[sentence[i]].srcW;  // ширина символа на текстуре
			float height = m_charDesc[sentence[i]].srcH; // высота символа на текстуре
			float xOff = m_charDesc[sentence[i]].xOff;
			float yOff = m_charDesc[sentence[i]].yOff;

			float left = drawX + xOff;
			float right = left + width * sizeLetter;
			float top = drawY - yOff * sizeLetter;
			float bottom = top - height * sizeLetter;
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
			drawX += m_charDesc[sentence[i]].xAdv * sizeLetter;
		}
	}

	// нарисовать текст данный в шейдер
	void Font::draw(unsigned int ind, XMFLOAT4 color, XMFLOAT2 pos)
	{
		m_shader->render(deviceContext, ind, color, pos, m_orthoMatrix);
	}

	void Text::init(const char* str, int numChars, bool m_dynamic_)
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

		pFont->buildVertexArray(vertex, m_numVert, str); // построить текст по данному шрифту (создать координаты текстур)
		m_numInd = 6 * max_charsize;
		unsigned long* indices = new unsigned long[m_numInd];
		for (int i = 0; i < max_charsize; i++)
		{
			indices[i * 6] = i * 4;
			indices[i * 6 + 1] = i * 4 + 1;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4;
			indices[i * 6 + 4] = i * 4 + 3;
			indices[i * 6 + 5] = i * 4 + 1;
		}

		m_vertexBuffer = Buffer::createVertexBuffer(pFont->getDevice(), sizeof(VertexFont) * m_numVert, vertex, m_dynamic);
		m_indexBuffer = Buffer::createIndexBuffer(pFont->getDevice(), sizeof(DWORD) * m_numInd, indices, m_dynamic);

		delete[] vertex;
		delete[] indices;
	}

	void Text::render(XMFLOAT4 color, XMFLOAT2 pos)
	{
		renderBuffers();
		pFont->draw(m_strlen * 6, color, pos);
	}

	void Text::setText(const char* text)
	{
		if (m_dynamic)
		{
			m_strlen = strlen(text);
			if (m_strlen > max_charsize)
			{
				m_strlen = max_charsize;
			}
			ID3D11DeviceContext* context = pFont->getContext();
			D3D11_MAPPED_SUBRESOURCE mr;
			HRESULT hr = context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
			VertexFont* vertPtr = (VertexFont*)mr.pData;
			pFont->buildVertexArray(vertPtr, m_numVert, text);
			context->Unmap(m_vertexBuffer, 0);
		}
		else
		{
			Log::Get()->Error("Attempt to change static text!");
		}
	}


	void Text::renderBuffers()
	{
		unsigned int stride = sizeof(VertexFont);
		unsigned int offset = 0;
		ID3D11DeviceContext* context = pFont->getContext();
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}