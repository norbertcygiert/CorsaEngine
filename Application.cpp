#include "Application.h"
#include <ctime>
void Application::frame() {
	wnd.accessGraphics().clearBuffer(.0f, .0f, .5f);
	wnd.accessGraphics().drawTriangle(-10.0f);
	wnd.accessGraphics().endFrame();
}
Application::Application() : wnd(1280, 960, "AstraEngine") {};
int Application::start() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		frame();
	}
}