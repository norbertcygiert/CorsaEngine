#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "GraphicsMacros.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"


#pragma comment(lib, "d3d11.lib")  //We dont have to change the linker settings this way.
#pragma comment(lib, "D3DCompiler.lib")


namespace wrl = Microsoft::WRL;
namespace dx = DirectX;


Graphics::Graphics(HWND hWnd) {
	//https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //Setting this flag makes the driver select the most efficient presentation technique
	sd.Flags = 0;
	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT cc;

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	// Gain access to texture subresource in the swap-chain
	wrl::ComPtr<ID3D11Resource> pBackBuf;
	//https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuf));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuf.Get(), nullptr, &pTarget));

	//Depth testing
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = 1;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsd, &pDSState));
	pContext->OMSetDepthStencilState(pDSState.Get(), 1);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC dd = {};
	dd.Width = SCREEN_W;
	dd.Height = SCREEN_H;
	dd.MipLevels = 1;
	dd.ArraySize = 1;
	dd.Format = DXGI_FORMAT_D32_FLOAT; //32bit float for all depth values
	dd.SampleDesc.Count = 1;
	dd.SampleDesc.Quality = 0;
	dd.Usage = D3D11_USAGE_DEFAULT;
	dd.CPUAccessFlags = 0;
	dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&dd, nullptr, pDepthStencil.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dDSV = {};
	dDSV.Format = DXGI_FORMAT_D32_FLOAT;
	dDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dDSV.Texture2D.MipSlice = 0;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dDSV, &pDSV));
	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDSV.Get());

	D3D11_VIEWPORT vp;
	vp.Width = float(SCREEN_W);
	vp.Height = float(SCREEN_H);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}


void Graphics::endFrame() {
	if (ImGuiEnabled) {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	HRESULT cc;
#ifndef NDEBUG
	infoManager.set();
#endif // !NDEBUG
	//Syncinterval is basically the value which our device framerate will be divided by, giving us the target FR.
	if (FAILED(cc = pSwap->Present(1u, 0u))) {
		if (cc == DXGI_ERROR_DEVICE_REMOVED) {
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else throw GFX_EXCEPT(cc);
	}
}

void Graphics::beginFrame(float r, float g, float b) noexcept {

	if (ImGuiEnabled) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::drawIndexed(unsigned int count) noexcept (!IS_DEBUG) {
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0, 0));
}

void Graphics::enableImGui() noexcept {
	ImGuiEnabled = true;
}

void Graphics::disableImGui() noexcept {
	ImGuiEnabled = false;
}

bool Graphics::isImGuiEnabled() const noexcept {
	return ImGuiEnabled;
}

void Graphics::setCamera(DirectX::FXMMATRIX c) noexcept {
	camera = c;
}

DirectX::XMMATRIX Graphics::getCamera() const noexcept {
	return camera;
}

void Graphics::setProjection(DirectX::FXMMATRIX p) noexcept{ projection = p; }

dx::XMMATRIX Graphics::getProjection() const noexcept { return projection; }

Graphics::HRESException::HRESException(int line, const char* file, HRESULT hr, std::vector<std::string> infoVec) noexcept : Exception(line, file), hr(hr) {
	for (const auto& m : infoVec) {
		info += m;
		info.push_back('\n');
	}
	if (!info.empty()) {
		info.pop_back();
	}
}

const char* Graphics::HRESException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error Code] 0x" << std::hex << getErrorCode() << std::dec << "(" << getErrorCode() << ")" << std::endl
		<< "[Description] " << getErrorDescription() << std::endl
		<< getOriginString();
	buf = oss.str();
	return buf.c_str();
}

const char* Graphics::HRESException::getType() const noexcept {
	return "Corsa Engine: Graphics Exception";
}

HRESULT Graphics::HRESException::getErrorCode() const noexcept { return hr; }

std::string Graphics::HRESException::getErrorDescription() const noexcept
{
	//Here normally we would use DXGetErrorString(), but Microsoft decided to get rid of the dxerr.h library so we have to make do 
	//with the Windows alternative, so just FormatMessageA (it has to be the ANSI (narrow char) version).
	//Now we also don't need the getErrorString function featured in dxerr.h
	char* msgBuf = nullptr;
	DWORD size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&msgBuf,
		0,
		nullptr
	);

	std::string msg = (size && msgBuf) ? std::string(msgBuf) : "Unidentified error code.";
	if (msgBuf) {
		LocalFree(msgBuf);
	}
	return msg;
}

std::string Graphics::HRESException::getErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::getType() const noexcept {
	return "Corsa Engine: Device Removed (DXGI_ERROR_DEVICE_REMOVED) Exception";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoVec) noexcept : Exception(line, file) {
	for (const auto& m : infoVec) {
		info += m;
		info.push_back('\n');
	}
	if (!info.empty()) {
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Additional Info] " << getErrorInfo() << std::endl
		<< "[Description] " << getErrorDescription() << std::endl
		<< getOriginString();
	buf = oss.str();
	return buf.c_str();
}

const char* Graphics::InfoException::getType() const noexcept { return "Corsa Engine: Info Exception"; }

std::string Graphics::InfoException::getErrorDescription() const noexcept {
	char* msgBuf = nullptr;
	DWORD size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		0,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&msgBuf,
		0,
		nullptr
	);

	std::string msg = (size && msgBuf) ? std::string(msgBuf) : "Unidentified error code.";
	if (msgBuf) {
		LocalFree(msgBuf);
	}
	return msg;
}

std::string Graphics::InfoException::getErrorInfo() const noexcept { return info; }
