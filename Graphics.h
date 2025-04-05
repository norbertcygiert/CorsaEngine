#pragma once
#include "BetterWin.h"
#include <d3d11.h>
class Graphics {
private:
    ID3D11Device* pDevice = nullptr;
    IDXGISwapChain* pSwap = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    ID3D11RenderTargetView* pTarget = nullptr;
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    ~Graphics();
    void endFrame();
    void clearBuffer(float r, float g, float b) noexcept {
        const float color[] = { r, g, b, 1.0f };
        pContext->ClearRenderTargetView(pTarget, color);
    }

};