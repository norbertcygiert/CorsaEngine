#pragma once
#include "Graphics.h"

class Camera {
private:
	float r = 20.f, theta = .0f, phi = .0f, pitch = .0f, yaw = .0f, roll = .0f;

public:
	DirectX::XMMATRIX getMatrix() const noexcept;
	void spawnControlWindow() noexcept;
	void resetCamera() noexcept;
};