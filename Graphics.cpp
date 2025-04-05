#include "Graphics.h"
#include <sstream>
#pragma comment(lib, "d3d11.lib") 
//This makes the linker actually link the d3d11.lib as including the header file only gets us the definitions.
//We also dont have to change the linker settings this way.

//We need an HRESULT codecatcher in scope for these macros two work.
#define GFX_THROW_FAILED(call) if(FAILED(cc = (call))) throw Graphics::HRESException(__LINE__, __FILE__, cc)
#define GFX_DEVICE_REMOVED_EXCEPT(cc) Graphics::DeviceRemovedException(__LINE__, __FILE__, (cc))

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

    HRESULT cc;

    GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
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
    ID3D11Resource* pBackBuf = nullptr;
    //https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
    GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuf)));
    GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuf, nullptr, &pTarget));
    pBackBuf->Release();
}

Graphics::~Graphics() {
    //Order of freeing these doesnt really matter beacause of reference counting in COM
    if (pTarget != nullptr) {
        pTarget->Release();
    }
    if (pContext != nullptr) {
        pContext->Release();
    }
    if (pSwap != nullptr) {
        pSwap->Release();
    }
    if (pDevice != nullptr) {
        pDevice->Release();
    }
}

void Graphics::endFrame(){
    HRESULT cc;
    if (FAILED(cc = pSwap->Present(2u, 0u))) //Syncinterval is basically the value which our device framerate will be divided by, giving us the target FR.
    {
        if (cc == DXGI_ERROR_DEVICE_REMOVED) {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else GFX_THROW_FAILED(cc);
    }
}

Graphics::HRESException::HRESException(int line, const char* file, HRESULT hr) noexcept : Exception(line, file), hr(hr) {}


const char* Graphics::DeviceRemovedException::getType() const noexcept {
    return "Astra Engine Exception: Device Removed (DXGI_ERROR_DEVICE_REMOVED)";
}

const char* Graphics::HRESException::what() const noexcept {
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[Error Code] " << getErrorCode() << std::endl
        << "[Description] " << getErrorDescription() << std::endl
        << getOriginString();
    buf = oss.str();
   return buf.c_str();
}

const char* Graphics::HRESException::getType() const noexcept {
    return "Astra Engine Exception: HRESException";
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

