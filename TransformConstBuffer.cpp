#include "TransformConstBuffer.h"

TransformConstBuffer::TransformConstBuffer(Graphics& g, const Drawable& parent) : parent(parent) {
	if (!vcBuf) { vcBuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(g); };
};

void TransformConstBuffer::bind(Graphics& g) noexcept {
	vcBuf->update(g, DirectX::XMMatrixTranspose( parent.getTransformXM() * g.getCamera() * g.getProjection() ) ); 
	vcBuf->bind(g);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformConstBuffer::vcBuf;