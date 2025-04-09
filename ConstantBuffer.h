#pragma once
#include "Bindable.h"
#include "GraphicsMacros.h"

template<typename T>
class ConstantBuffer : public Bindable {
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
public:
	void update(Graphics& g, const T& consts) {
		INFO_INSERT(g);

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(getContext(g)->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		));
		memcpy(msr.pData, &consts, sizeof(consts));
		getContext(g)->Unmap(pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics& g, const T& consts) {
		INFO_INSERT(g);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(getDevice(g)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	ConstantBuffer(Graphics& g) {
		INFO_INSERT(g);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(getDevice(g)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}

};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T> {
	using ConstantBuffer<T>::pConstantBuffer;
	using Bindable::getContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void bind(Graphics& g) noexcept override {
		getContext(g)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T> {
	using ConstantBuffer<T>::pConstantBuffer;
	using Bindable::getContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void bind(Graphics& g) noexcept override {
		getContext(g)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};