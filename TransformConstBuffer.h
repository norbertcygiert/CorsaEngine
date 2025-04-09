#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformConstBuffer : public Bindable {
private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcBuf;
	const Drawable& parent;
public:
	TransformConstBuffer(Graphics& g, const Drawable& parent);
	void bind(Graphics& g) noexcept override;
};