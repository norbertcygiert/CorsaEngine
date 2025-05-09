#include "IndexBuffer.h"
#include "GraphicsMacros.h"

IndexBuffer::IndexBuffer(Graphics& g, const std::vector<unsigned short>& indices) : count((unsigned int) indices.size()) {
	INSERT_CODECATCHER(g);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = (unsigned int)(count * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	GFX_THROW_INFO(getDevice(g)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
}

void IndexBuffer::bind(Graphics& g) noexcept { getContext(g)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u); }

unsigned int IndexBuffer::getCount() const noexcept { return count; }