#include "Graphics.h"

#pragma comment(lib, "d3d11.lib") 
//This makes the linker actually link the d3d11.lib as including the header file only gets us the definitions.
//We also dont have to change the linker settings this way.

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

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwap,
        &pDevice,
        nullptr,
        &pContext
    );

    // Gain access to texture subresource in the swap-chain
    ID3D11Resource* pBackBuf = nullptr;
    //https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
    pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuf));
    pDevice->CreateRenderTargetView(pBackBuf, nullptr, &pTarget);
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
    pSwap->Present(2u, 0u); //Syncinterval is basically the value which our device framerate will be divided by, giving us the target FR.
}
