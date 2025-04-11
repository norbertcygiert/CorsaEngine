#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
public:
	VertexShader(Graphics& g, const std::wstring& path);
	void bind(Graphics& g) noexcept override;
	ID3DBlob* getBytecode() const noexcept;
};
