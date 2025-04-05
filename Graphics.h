#pragma once
#include "BetterWin.h"
#include <d3d11.h>
#include <string>
#include "Exceptions.h"
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


	class Exception : public AstraException
	{
		using AstraException::AstraException;
	public:
		static std::string translateErrorCode(HRESULT hr) noexcept;
	};

	class HRESException : public Exception
	{
	public:
		HRESException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};

	class DeviceRemovedException : public HRESException {
		using HRESException::HRESException;
	public:
		const char* getType() const noexcept override;
	};
};