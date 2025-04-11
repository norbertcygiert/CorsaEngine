#include "Sampler.h"
#include "GraphicsMacros.h"

Sampler::Sampler(Graphics& g) {
	INSERT_CODECATCHER(g);
	//SamplerState are instructions for the pipeline on how to perform texture lookups
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//This matters when our textures get out of basic <0,1> range
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(getDevice(g)->CreateSamplerState(&sd, &pSampler));
}

void Sampler::bind(Graphics& gfx) noexcept {
	getContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}