#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
public:
	Texture(Graphics& g, const class Surface& s);
	void bind(Graphics& g) noexcept override;

};
