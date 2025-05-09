#pragma once
#include "DrawableBase.h"
#include <memory>

class Box : public DrawableBase<Box> {
public:
	Box(Graphics& gfx, std::default_random_engine& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist);
	void update(float dt) noexcept override;
	DirectX::XMMATRIX getTransformXM() const noexcept override;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
	DirectX::XMFLOAT3X3 mt;
};