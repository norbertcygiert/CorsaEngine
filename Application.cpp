#include "Application.h"

void Application::frame() {
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