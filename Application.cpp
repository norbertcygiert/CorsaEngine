#include "Application.h"
#include "Box.h"
#include "Cube.h"
#include "Ball.h"
#include "Sheet.h"
#include "Math.h"
#include <memory>
#include <algorithm>
#include <random>
#include "GDIPlus.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

GDIPlus gdi;

Application::Application() : wnd(SCREEN_W, SCREEN_H, "CorsaEngine") {
	class Factory {
	private:
		Graphics& g;
		std::default_random_engine rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,2 };
	public:
		Factory(Graphics& g) : g(g) {};
		std::unique_ptr<Drawable> operator()() {
			switch (typedist(rng)) {
			case 0:
				return std::make_unique<Box>(
					g, rng, adist, ddist,
					odist, rdist, bdist
				);
			case 1:
				return std::make_unique<Ball>(
					g, rng, adist, ddist,
					odist, rdist, longdist, latdist
				);
			case 2:
				return std::make_unique<Sheet>(
					g, rng, adist, ddist,
					odist, rdist
				);
			default:
				assert(false && "Application.cpp: Wrong drawable type passed into the Factory");
				return {};
			}
		}
	
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
		d->update(wnd.keybd.keyDown(VK_SPACE) ? .0f : 0.01f); //Press space to pause
		d->draw(wnd.accessGraphics());
	}
	//ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	if (show) {
		ImGui::ShowDemoWindow(&show);
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	//
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