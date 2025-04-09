#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable {
protected:
	unsigned int count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
public:
	IndexBuffer(Graphics& g, const std::vector<unsigned short>& indices);
	void bind(Graphics& g) noexcept override;
	unsigned int getCount() const noexcept;
};
