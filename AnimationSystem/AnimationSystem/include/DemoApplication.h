// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#pragma once
#include "Window.h"
#include "gl/Program.h"
#include <memory>

class Application
{
public:
	Application();
	~Application();

	void draw();

	void update();

private:
	std::shared_ptr<Window> m_window;
};
