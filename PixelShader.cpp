#include "PixelShader.h"
#include "GraphicsMacros.h"

PixelShader::PixelShader(Graphics& g, const std::wstring& path)
{
	INSERT_CODECATCHER(g);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	GFX_THROW_INFO(getDevice(g)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::bind(Graphics& g) noexcept
{
	getContext(g)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}