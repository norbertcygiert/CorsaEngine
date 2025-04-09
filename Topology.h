#pragma once
#include "Bindable.h"
class Topology : public Bindable {
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
public:
	Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type);
	void bind(Graphics& g) noexcept override;
};