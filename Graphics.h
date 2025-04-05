#pragma once
#include "BetterWin.h"
#include "Exceptions.h"
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <vector>
#include "DXGIInfoManager.h"

class Graphics {
private:
#ifndef NDEBUG
	DXGIInfoManager infoManager;
#endif // !NDEBUG
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void endFrame();
	void clearBuffer(float r, float g, float b) noexcept;

	class Exception : public AstraException
	{
		using AstraException::AstraException;
	public:
		static std::string translateErrorCode(HRESULT hr) noexcept;
	};

	class HRESException : public Exception
	{
	public:
		HRESException(int line, const char* file, HRESULT hr, std::vector<std::string> infoVec) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorDescription() const noexcept;
		std::string getErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public HRESException {
		using HRESException::HRESException;
	public:
		const char* getType() const noexcept override;
	private:
		std::string reason;
	};
};