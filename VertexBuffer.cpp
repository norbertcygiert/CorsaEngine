#include "VertexBuffer.h"

void VertexBuffer::bind(Graphics& gfx) noexcept {
	const unsigned int offset = 0u;
	getContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}