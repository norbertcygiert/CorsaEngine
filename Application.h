#pragma once

#include "Window.h"
#include "ImGuiWrapper.h"
#include "Camera.h"

#define SCREEN_W 1280
#define SCREEN_H 960
static constexpr size_t nDrawables = 180;
class Application {
private:
	IMGUIWrapper imgui;
	Window wnd;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	Camera camera;
	void frame(); //Being called on every frame
public:
	Application();
	~Application();
	int start(); //Initializing the application
	std::vector<std::unique_ptr<class Box>> boxes;
};