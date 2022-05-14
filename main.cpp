#include "main.h"
#include <omp.h>
#include <iostream>
//#include"saverBMP.h"


int main(int argc, char** argv)
{
	MyEngine::Engine engine;

	engine.init();
	engine.run();

	//system("pause");

	return 0;
}