#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <vector>
#include "IndexBuffer.h"

class Bindable;

class Drawable {
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;
	virtual DirectX::XMMATRIX getTransformXM() const noexcept = 0;
	void draw(Graphics& g) const noexcept(!IS_DEBUG);
	virtual void update(float dt) noexcept = 0;

protected:
	void addBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	void addIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept(!IS_DEBUG);
	
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& getStaticBinds() const noexcept = 0;
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;

};