#include "Texture.h"
#include "Surface.h"
#include "GraphicsMacros.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& g, const Surface& s) {
	INSERT_CODECATCHER(g);

	D3D11_TEXTURE2D_DESC td = {};
	td.Width = s.getWidth();
	td.Height = s.getHeight();
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = s.getBufferPtr();
	//Pitch is the distance in bytes between vertically adjacent pixels
	//e.g First pixel in row 1 and First pixel in row 2
	sd.SysMemPitch = s.getWidth() * sizeof(Surface::Color); 
	wrl::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO(getDevice(g)->CreateTexture2D( &td, &sd, &pTexture ));

	// Create the resource view on the texture
	// We create it as textures themselves arent bound to the actual pipeline
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = td.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(getDevice(g)->CreateShaderResourceView( pTexture.Get(), &srvDesc, &pTextureView ));
}

void Texture::bind(Graphics& gfx) noexcept {
	getContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}