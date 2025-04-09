#pragma once
#include "Bindable.h"

class PixelShader : public Bindable
{
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
public:
	PixelShader(Graphics& g, const std::wstring& path);
	void bind(Graphics& g) noexcept override;
};
