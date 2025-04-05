#pragma once

#include "Window.h"

class Application {
private:
	Window wnd;
	void frame();
public:
	Application();
	int start();
};