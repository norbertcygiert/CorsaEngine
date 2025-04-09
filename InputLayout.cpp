#include "InputLayout.h"
#include "GraphicsMacros.h"

InputLayout::InputLayout(Graphics& g, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode){
	INFO_INSERT(g);
	GFX_THROW_INFO(getDevice(g)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	));
}

void InputLayout::bind(Graphics& g) noexcept{ getContext(g)->IASetInputLayout(pInputLayout.Get()); }