#include "Graphics.h"
#include <sstream>
#pragma comment(lib, "d3d11.lib") 
//This makes the linker actually link the d3d11.lib as including the header file only gets us the definitions.
//We also dont have to change the linker settings this way.

namespace wrl = Microsoft::WRL;

//We need an HRESULT codecatcher in scope for these macros two work.
#define GFX_EXCEPT_NOINFO(cc) Graphics::HRESException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_NOINFO(cc) if( FAILED( cc = (call) ) ) throw Graphics::HRESException( __LINE__,__FILE__,cc )

#ifndef NDEBUG
#define GFX_EXCEPT(cc) Graphics::HRESException( __LINE__,__FILE__,(cc),infoManager.getMessages() )
#define GFX_THROW_INFO(call) infoManager.set(); if( FAILED( cc = (call) ) ) throw GFX_EXCEPT(cc)
#define GFX_DEVICE_REMOVED_EXCEPT(cc) Graphics::DeviceRemovedException( __LINE__,__FILE__,(cc),infoManager.getMessages() )
//#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.getMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(cc) Graphics::HRESException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_INFO(call) GFX_THROW_NOINFO(call)
#define GFX_DEVICE_REMOVED_EXCEPT(cc) Graphics::DeviceRemovedException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

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
    sd.OutputWindow = (HWND)696969;
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
}


void Graphics::endFrame() {
    HRESULT cc;
#ifndef NDEBUG
    infoManager.set();
#endif // !NDEBUG

    if (FAILED(cc = pSwap->Present(1u, 0u))) //Syncinterval is basically the value which our device framerate will be divided by, giving us the target FR.
    {
        if (cc == DXGI_ERROR_DEVICE_REMOVED) {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else throw GFX_EXCEPT(cc);
    }
}

void Graphics::clearBuffer(float r, float g, float b) noexcept{
    const float color[] = { r, g, b, 1.0f };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
}

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
        << "[Error Code] 0x" << std::hex << getErrorCode()  << std::dec << "(" << getErrorCode() << ")" << std::endl
        << "[Description] " << getErrorDescription() << std::endl
        << getOriginString();
    buf = oss.str();
    return buf.c_str();
}

const char* Graphics::HRESException::getType() const noexcept {
    return "Astra Engine Graphics Exception";
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
    return "Astra Engine Exception: Device Removed (DXGI_ERROR_DEVICE_REMOVED)";
}