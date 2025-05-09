#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"


template<class T>
class DrawableBase : public Drawable {
protected:
	bool isStaticInitialized() const noexcept{
		return !staticBinds.empty();
	}
	void addStaticBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG){
		assert(typeid(*bind) != typeid(IndexBuffer) && "Must use addIndexBuffer to bind index buffer");
		staticBinds.push_back(std::move(bind));
	}
	void addStaticIndexBuffer(std::unique_ptr<IndexBuffer> indexBuf) noexcept(!IS_DEBUG){
		assert(pIndexBuffer == nullptr && "Attempting to add index buffer a second time");
		pIndexBuffer = indexBuf.get();
		staticBinds.push_back(std::move(indexBuf));
	}
	void setIndexFromStatic() noexcept(!IS_DEBUG) {
		assert(pIndexBuffer == nullptr && "Attempting to add index buffer a second time");
		for (const auto& b : staticBinds) {
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get())) {
				pIndexBuffer = p;
				return;
			}
		}
		assert(pIndexBuffer != nullptr && "Failed to find index buffer in static binds");
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& getStaticBinds() const noexcept override { return staticBinds; }
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;
