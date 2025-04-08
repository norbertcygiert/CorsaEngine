#pragma once

#include "Window.h"
#define SCREEN_W 1280
#define SCREEN_H 960
class Application {
private:
	Window wnd;
	void frame(); //Being called on every frame
public:
	Application();
	int start(); //Initializing the application
};