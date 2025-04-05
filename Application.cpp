#include "Application.h"

void Application::frame() {
	
}
Application::Application() : wnd(800, 600, "AstraEngine") {};
int Application::start() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		frame();
	}
}