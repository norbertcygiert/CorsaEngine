#pragma once
#include "DrawableBase.h"

class Ball : public DrawableBase<Ball> {
public:
	Ball(Graphics& gfx, std::default_random_engine& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_int_distribution<int>& longdist,
		std::uniform_int_distribution<int>& latdist);
	void update(float dt) noexcept override;
	DirectX::XMMATRIX getTransformXM() const noexcept override;
private:
	// Position
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// Speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};
