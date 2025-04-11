#pragma once
#include "Bindable.h"
#include "GraphicsMacros.h"

class VertexBuffer : public Bindable {
protected:
	unsigned int stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
public:
	template<class T>
	VertexBuffer(Graphics& g, const std::vector<T>& vertices) : stride(sizeof(T)) {
		INSERT_CODECATCHER(g);
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(T) * vertices.size());
		bd.StructureByteStride = sizeof(T);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_INFO(getDevice(g)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	void bind(Graphics& g) noexcept override;
};
