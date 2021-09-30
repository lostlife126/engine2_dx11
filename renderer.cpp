#include "renderer.h"

namespace MyEngine
{
	void Renderer::renderScene(float dt, double time)
	{

	//		std::stringstream ss;
	//		ss <<"fps:"<< 1.0f/dt;
	//		std::string str;
	//		ss >> str;
	//	text_fps->setText(driverDX11->deviceContext, str.c_str());
	//	text_fps->render(driverDX11->deviceContext, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

	//	std::stringstream ss2;
	//	ss2 <<"time:"<< time;
	//	ss2 >> str;

	//	text_time->setText(driverDX11->deviceContext, str.c_str());
	//	text_time->render(driverDX11->deviceContext, 1.0f, 1.0f, 1.0f, 0.0f, 100.0f);

		return;
	}

	void Renderer::init(HWND hwnd_)
	{
		hwnd = hwnd_;
		driverDX11 = new VideoDriverDX11;
		driverDX11->init(hwnd);
	//	shader.initShaders(driverDX11->m_pDevice, "mesh.vs", "mesh.ps");

	//	font.init(driverDX11->m_pDevice, driverDX11->width_screen, driverDX11->height_screen);
	//	text_fps = new Text(&font);
	//	text_fps->init(driverDX11->m_pDevice, "fps = ", 15, true);

	//	text_time = new Text(&font);
	//	text_time->init(driverDX11->m_pDevice, "time = ", 20, true);

		return;
	}
}