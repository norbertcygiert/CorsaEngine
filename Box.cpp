#include "Box.h"
#include "BindableBase.h"
#include "GraphicsMacros.h"
#include "Sphere.h"
#include "Cube.h"
namespace dx = DirectX;
Box::Box(Graphics& g, std::default_random_engine& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist )
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
		};
		const auto model = Cube::make<Vertex>();

		addStaticBind(std::make_unique<VertexBuffer>(g, model.vertices));

		auto pvs = std::make_unique<VertexShader>(g, L"VertexShader.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<PixelShader>(g, L"PixelShader.cso"));

		addStaticIndexBuffer(std::make_unique<IndexBuffer>(g, model.indices));

		struct PSConsts { //PixelShader constans
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
	else setIndexFromStatic();
	addBind(std::make_unique<TransformConstBuffer>(g, *this));
	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

void Box::update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

dx::XMMATRIX Box::getTransformXM() const noexcept
{
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}