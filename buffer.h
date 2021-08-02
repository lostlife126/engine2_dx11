#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

namespace MyEngine
{
	class Buffer
	{
	public:

		static ID3D11Buffer* createVertexBuffer(ID3D11Device* device, int size, const void* data, bool dynamic);
		static ID3D11Buffer* createIndexBuffer(ID3D11Device* device, int size, const void* data, bool dynamic);
		static ID3D11Buffer* createConstantBuffer(ID3D11Device* device, int size, bool dynamic);
	};


	/*
	const int gammaTable[256] = { 0, 21, 28, 34, 39, 43, 46,
			50, 53, 56, 59, 61, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84,
			85, 87, 89, 90, 92, 93, 95, 96, 98, 99, 101, 102, 103, 105, 106,
			107, 109, 110, 111, 112, 114, 115, 116, 117, 118, 119, 120, 122,
			123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
			136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147,
			148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 156, 157, 158,
			159, 160, 160, 161, 162, 163, 164, 164, 165, 166, 167, 167, 168,
			169, 170, 170, 171, 172, 173, 173, 174, 175, 175, 176, 177, 178,
			178, 179, 180, 180, 181, 182, 182, 183, 184, 184, 185, 186, 186,
			187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195,
			195, 196, 197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203,
			203, 204, 205, 205, 206, 206, 207, 207, 208, 209, 209, 210, 210,
			211, 212, 212, 213, 213, 214, 214, 215, 215, 216, 217, 217, 218,
			218, 219, 219, 220, 220, 221, 221, 222, 223, 223, 224, 224, 225,
			225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231,
			232, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238,
			238, 239, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244,
			245, 245, 246, 246, 247, 247, 248, 248, 249, 249, 249, 250, 250,
			251, 251, 252, 252, 253, 253, 254, 254, 255, 255 };

#include <vector>

	template <typename T>
	class Buffer
	{
	public:
		int width;
		int height;
		int size;
		std::vector<T> data;

		Buffer() : width(0), height(0), size(0) {}

		Buffer(int w_, int h_) :width(w_), height(h_), size(w_* h_)
		{
			data.resize(size);
			nulling();
		}

		void resize(int w_, int h_)
		{
			width = w_;
			height = h_;
			size = w_ * h_;
			data.resize(size);
			nulling();
		}

		void nulling()
		{
			for (auto x : data)
			{
				if (std::is_same<T, float>::value)
				{
					x = 0.0f;
				}
				else
				{
					x = 0;
				}
			}
		}

		T& operator()(int j, int i)
		{
			return data[i + j * width];
		}

		const T& operator()(int j, int i) const
		{
			return data[i + j * width];
		}

		void saveBMP(const std::string& path)
		{
#pragma pack(1)
			struct
			{
				char type[2];
				int size;
				int reserved;
				int offset;
			}BITMAPFILEHEADER;
			struct
			{
				int size;
				int width;
				int height;
				short planes;
				short bitCount;
				int bitCompression;
				int sizeImage;
				int xpm;
				int ypm;
				int table;
				int offset;
			}BITMAPINFO;
#pragma pack(8)
			BITMAPFILEHEADER.type[0] = 'B';
			BITMAPFILEHEADER.type[1] = 'M';
			BITMAPFILEHEADER.size = 54 + 3 * size;
			BITMAPFILEHEADER.reserved = 0;
			BITMAPFILEHEADER.offset = 54;

			BITMAPINFO.size = 40;
			BITMAPINFO.width = width;
			BITMAPINFO.height = height;
			BITMAPINFO.planes = 1;
			BITMAPINFO.bitCount = 24;
			BITMAPINFO.bitCompression = 0;
			BITMAPINFO.sizeImage = 3 * size;
			BITMAPINFO.xpm = 1;
			BITMAPINFO.ypm = 1;
			BITMAPINFO.table = 0;
			BITMAPINFO.offset = 0;

			std::ofstream file(path.c_str(), std::ios::binary);
			file.write(reinterpret_cast<char*>(&BITMAPFILEHEADER), 14);
			file.write(reinterpret_cast<char*>(&BITMAPINFO), 40);

			int lenLine = width * 3 + ((width * 3) % 4);
			char* buff = new char[lenLine];

			for (int i = 0; i < height; i++)
			{
				memset(&(buff[lenLine - 3]), 0, 3);
				for (int j = 0; j < width; j++)
				{
					uint32_t val;

					if (std::is_same<T, float>::value)
					{
						val = (int((*this)(i, j) * 255.0)) * (1 + 256 + 256 * 256);
					}
					else
					{
						val = (*this)(i, j);
					}
					memcpy(&(buff[3 * j]), &val, 3);
				}
				file.write(buff, lenLine);
			}
			delete[] buff;
			file.close();
		}

		void loadBMP(const std::string& path)
		{
#pragma pack(1)
			struct
			{
				char type[2];
				int size;
				int reserved;
				int offset;
			}BITMAPFILEHEADER;
			struct
			{
				int size;
				int width;
				int height;
				short planes;
				short bitCount;
				int bitCompression;
				int sizeImage;
				int xpm;
				int ypm;
				int table;
				int offset;
			}BITMAPINFO;
#pragma pack(8)
			std::ifstream file(path.c_str(), std::ios::binary);
			file.read(reinterpret_cast<char*>(&BITMAPFILEHEADER), 14);
			file.read(reinterpret_cast<char*>(&BITMAPINFO), 40);

			width = BITMAPINFO.width;
			height = BITMAPINFO.height;
			size = width * height;
			data.resize(size);


			int lenLine = width * 3 + ((width * 3) % 4);
			char* buff = new char[lenLine];

			for (int i = 0; i < height; i++)
			{
				file.read(buff, lenLine);

				for (int j = 0; j < width; j++)
				{
					int val = 0;
					memcpy(&val, &(buff[j * 3]), 3);

					if (std::is_same<T, float>::value)
					{
						(*this)(i, j) = (val % 256) / 255.0f;
					}
					else
					{
						(*this)(i, j) = val;
					}
				}
			}
			delete[] buff;
			file.close();
		}

	};

	class Texture : public Buffer<uint32_t>
	{
	public:
		Texture() :Buffer() {}

		uint32_t getPixel(float x, float y)
		{
			int i = int(x * width);
			int j = int(y * height);
			return (*this)(j, i);
		}

	};
	*/
}