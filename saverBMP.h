#pragma once
#include <vector>
#include <fstream>

class LoaderMRM
{
public:



	void load(std::string path)
	{
		std::ifstream file(path.c_str(), std::ios::binary);
		while (!file.eof())
		{
			short signature;
			int size;
			short signature2;
			int size2;
			float pos[200];
				/*
			short ss[36];
			float pp[20];
			int dd;
			float ds[2];
			short sd[2];
			float ff[5];
			int ii[2];
			float f4[4];
			short ssw[2];
			float f5[5];
			short ss_[2];
			float f5_[5];
			*/
			file.read(reinterpret_cast<char*>(&signature), 2);
			file.read(reinterpret_cast<char*>(&size), 4);
			file.read(reinterpret_cast<char*>(&signature2), 2);
			file.read(reinterpret_cast<char*>(&size2), 4);
			file.read(reinterpret_cast<char*>(pos), 4* 200);
		//	file.read(reinterpret_cast<char*>(ss), 2 * 36);
		//	file.read(reinterpret_cast<char*>(pp), 4 * 20);
		//	file.read(reinterpret_cast<char*>(&dd), 4);
		/*	file.read(reinterpret_cast<char*>(ds), 4 * 2);
			file.read(reinterpret_cast<char*>(sd), 2 * 2);
			file.read(reinterpret_cast<char*>(ff), 4 * 5);
			file.read(reinterpret_cast<char*>(ii), 2 * 4);
			file.read(reinterpret_cast<char*>(f4), 4 * 4);
			file.read(reinterpret_cast<char*>(ssw), 2 * 4);
			file.read(reinterpret_cast<char*>(f5), 4 * 4);
			file.read(reinterpret_cast<char*>(ss_), 2 * 4);
			file.read(reinterpret_cast<char*>(f5_), 5 * 4);
			*/
			//first = first;
			int g = 9;;
		}

	}
};