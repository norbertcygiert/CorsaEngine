#include "Bindable.h"

ID3D11DeviceContext* Bindable::getContext(Graphics& g) noexcept { return g.pContext.Get(); }

ID3D11Device* Bindable::getDevice(Graphics& g) noexcept { return g.pDevice.Get(); }

DXGIInfoManager& Bindable::getInfoManager(Graphics& g) noexcept(!IS_DEBUG) {
#ifndef NDEBUG
	return g.infoManager;
#else
	throw std::logic_error("Error: Tried to access graphics' infoManager in release mode, change to Debug and try again.");
#endif
}
