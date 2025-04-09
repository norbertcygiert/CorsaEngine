#include "TransformConstBuffer.h"

TransformConstBuffer::TransformConstBuffer(Graphics& gfx, const Drawable& parent) : vcBuf(gfx), parent(parent) {};

void TransformConstBuffer::bind(Graphics& gfx) noexcept {
	vcBuf.update(gfx, DirectX::XMMatrixTranspose( parent.getTransformXM() * gfx.getProjection() ) );
	vcBuf.bind(gfx);
}