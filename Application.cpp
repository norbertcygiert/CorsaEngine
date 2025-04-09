#include "Application.h"
#include "Box.h"
#include <memory>
#include <random>

Application::Application() : wnd(SCREEN_W, SCREEN_H, "AstraEngine") {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 50; i++){
		boxes.push_back(std::make_unique<Box>(
			wnd.accessGraphics(), rng, adist,
			ddist, odist, rdist
		));
	}
	wnd.accessGraphics().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 0.75f, 0.5f, 40.0f));
}

Application::~Application() {};

void Application::frame() {
	wnd.accessGraphics().clearBuffer(0.07f, .0f, .12f);
	for (auto& b : boxes) {
		b->update(0.01f);
		b->draw(wnd.accessGraphics());
	}
	wnd.accessGraphics().endFrame();
}
int Application::start() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		frame();
	}
}