#pragma once
#include "BetterWin.h"
#include "Exceptions.h"
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <vector>
#include "DXGIInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
namespace wrl = Microsoft::WRL;
#define SCREEN_W 1280
#define SCREEN_H 960


class Graphics {
	friend class Bindable;
private:
#ifndef NDEBUG
	DXGIInfoManager infoManager;
#endif // !NDEBUG
	wrl::ComPtr<ID3D11Device> pDevice = nullptr;
	wrl::ComPtr<IDXGISwapChain> pSwap = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	DirectX::XMMATRIX projection;
	bool ImGuiEnabled = true;

public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void endFrame();
	void beginFrame(float r, float g, float b) noexcept;
	void drawIndexed(unsigned int count) noexcept (!IS_DEBUG);
	void setProjection(DirectX::FXMMATRIX p) noexcept;
	void enableImGui() noexcept;
	void disableImGui() noexcept;
	bool isImGuiEnabled() const noexcept;
	DirectX::XMMATRIX getProjection() const noexcept;

	class Exception : public CorsaException {
		using CorsaException::CorsaException;
	public:
		static std::string translateErrorCode(HRESULT hr) noexcept;
	};

	class HRESException : public Exception {
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

	class InfoException : public Exception {
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoVec) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		std::string getErrorDescription() const noexcept;
		std::string getErrorInfo() const noexcept;
	private:
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