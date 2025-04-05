#pragma once

#include "Window.h"

class Application {
private:
	Window wnd;
	void frame(); //Being called on every frame
public:
	Application();
	int start(); //Initializing the application
};