#include "Drawable.h"
#include "GraphicsMacros.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::draw(Graphics& g) const noexcept(!IS_DEBUG)
{
	for (auto& b : binds) {
		b->bind(g);
	}
	for (auto& b : getStaticBinds()) {
		b->bind(g);
	}
	g.drawIndexed(pIndexBuffer->getCount());
}

void Drawable::addBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG) {
	assert("Must use addIndexBuffer() to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::addIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!IS_DEBUG) {
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}