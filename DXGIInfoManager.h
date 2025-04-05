#pragma once
#include "BetterWin.h"
#include <wrl.h>
#include <vector>
#include <string>
#include <dxgidebug.h>

class DXGIInfoManager {
public:
	DXGIInfoManager();
	~DXGIInfoManager() = default;
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
	void set() noexcept;
	std::vector<std::string> getMessages() const;
private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDXGIInfoQueue = nullptr;
};