#include "Topology.h"
#include "Bindable.h"

Topology::Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type) : type(type) {}

void Topology::bind(Graphics& g) noexcept {
	getContext(g)->IASetPrimitiveTopology(type);
}