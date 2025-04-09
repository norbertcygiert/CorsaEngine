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
	~Application();
	int start(); //Initializing the application
	std::vector<std::unique_ptr<class Box>> boxes;
};