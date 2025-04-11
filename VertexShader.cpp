#include "VertexShader.h"
#include "GraphicsMacros.h"

VertexShader::VertexShader(Graphics& g, const std::wstring& path) {
	INSERT_CODECATCHER(g);
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
	GFX_THROW_INFO(getDevice(g)->CreateVertexShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	));
}

void VertexShader::bind(Graphics& gfx) noexcept { getContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u); }

ID3DBlob* VertexShader::getBytecode() const noexcept { return pBytecodeBlob.Get(); }