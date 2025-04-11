#include "Application.h"
#include "Box.h"
#include "Cube.h"
#include "Ball.h"
#include "Math.h"
#include <memory>
#include <algorithm>
#include <random>

Application::Application() : wnd(SCREEN_W, SCREEN_H, "AstraEngine") {
	class Factory {
	public:
		Factory(Graphics& g) : g(g) {};
		std::unique_ptr<Drawable> operator()() {
			switch (typedist(rng)) {
			case 0:
				return std::make_unique<Ball>(
					g, rng, adist, ddist,
					odist, rdist, longdist, latdist
				);
			case 1:
				return std::make_unique<Box>(
					g, rng, adist, ddist,
					odist, rdist, bdist
				);
			default:
				assert(false && "Wrong drawable type passed into the Factory");
				return {};
			}
		}
	private:
		Graphics& g;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,1 };
	};

	Factory factory(wnd.accessGraphics());
	drawables.reserve(nDrawables); //Fancy c++ vector features
	std::generate_n(std::back_inserter(drawables), nDrawables, factory);


	wnd.accessGraphics().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 0.75f, 0.5f, 40.0f));
}

Application::~Application() {};

void Application::frame() {
	wnd.accessGraphics().clearBuffer(0.07f, .0f, .12f);
	for (auto& d : drawables) {
		d->update(0.01f);
		d->draw(wnd.accessGraphics());
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