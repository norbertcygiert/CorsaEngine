#include "Sheet.h"
#include "BindableBase.h"
#include "GraphicsMacros.h"
#include "Plane.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"

namespace dx = DirectX;

Sheet::Sheet(Graphics& g, std::default_random_engine& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
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
		struct Vertex {
			dx::XMFLOAT3 pos;
			struct { 
				float u;
				float v;
			} tex;
		};
		auto model = Plane::make<Vertex>();
		model.vertices[0].tex = { 0.0f,0.0f };
		model.vertices[1].tex = { 1.0f,0.0f };
		model.vertices[2].tex = { 0.0f,1.0f };
		model.vertices[3].tex = { 1.0f,1.0f };

		addStaticBind(std::make_unique<Texture>(g, Surface::fromFile("Assets\\ketchup.png")));

		addStaticBind(std::make_unique<VertexBuffer>(g, model.vertices));

		addStaticBind(std::make_unique<Sampler>(g));

		auto pvs = std::make_unique<VertexShader>(g, L"VertexShaderTexture.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<PixelShader>(g, L"PixelShaderTexture.cso"));

		addStaticIndexBuffer(std::make_unique<IndexBuffer>(g, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		addStaticBind(std::make_unique<InputLayout>(g, ied, pvsbc));

		addStaticBind(std::make_unique<Topology>(g, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else setIndexFromStatic();

	addBind(std::make_unique<TransformConstBuffer>(g, *this));
}

void Sheet::update(float dt) noexcept {
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

dx::XMMATRIX Sheet::getTransformXM() const noexcept {
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
		//We don't need the last translation (0,0,20), because we now have an independent camera
}