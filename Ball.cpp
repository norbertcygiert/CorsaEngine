#include "Ball.h"
#include "BindableBase.h"
#include "GraphicsMacros.h"
#include "Sphere.h"

namespace dx = DirectX;

Ball::Ball(Graphics& g, std::default_random_engine& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<int>& longdist,
	std::uniform_int_distribution<int>& latdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if (!isStaticInitialized()) {
		auto pvs = std::make_unique<VertexShader>(g, L"VertexShader.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<PixelShader>(g, L"PixelShader.cso"));

		struct PSConsts {
			struct {
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PSConsts cb2 = {
			{
				{ 1.0f,1.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
				{ 0.0f,0.0f,0.0f },
			}
		};
		addStaticBind(std::make_unique<PixelConstantBuffer<PSConsts>>(g, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		addStaticBind(std::make_unique<InputLayout>(g, ied, pvsbc));

		addStaticBind(std::make_unique<Topology>(g, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	struct Vertex {
		dx::XMFLOAT3 pos;
	};
	auto model = Sphere::makeTesselated<Vertex>(latdist(rng), longdist(rng));
	// Deforming the vertices of model by linear transformation
	model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));

	addBind(std::make_unique<VertexBuffer>(g, model.vertices));

	addIndexBuffer(std::make_unique<IndexBuffer>(g, model.indices));

	addBind(std::make_unique<TransformConstBuffer>(g, *this));
}

void Ball::update(float dt) noexcept {
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

dx::XMMATRIX Ball::getTransformXM() const noexcept {
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}