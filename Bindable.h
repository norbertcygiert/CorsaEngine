#pragma once
#include "Graphics.h"

class Bindable {
public:
	virtual ~Bindable() = default;
	virtual void bind(Graphics& g) noexcept = 0;
protected:
	static ID3D11DeviceContext* getContext(Graphics& g) noexcept;
	static ID3D11Device* getDevice(Graphics& g) noexcept;
	static DXGIInfoManager& getInfoManager(Graphics& g) noexcept(!IS_DEBUG);
};