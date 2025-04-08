#include "Application.h"
#include <ctime>
void Application::frame() {
	wnd.accessGraphics().clearBuffer(.0f, .0f, .5f);
	wnd.accessGraphics().drawTriangle(0.0f,0.0f, 0.0f);
	wnd.accessGraphics().drawTriangle(1.0f, wnd.mouse.getX() / (static_cast<float>(SCREEN_W / 2)) - 1.f, -wnd.mouse.getY() / (static_cast<float>(SCREEN_H / 2)) + 1.f);
	wnd.accessGraphics().endFrame();
}
Application::Application() : wnd(SCREEN_W, SCREEN_H, "AstraEngine") {};
int Application::start() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		frame();
	}
}