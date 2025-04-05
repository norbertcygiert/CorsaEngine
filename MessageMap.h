#pragma once

#include "BetterWin.h"
#include <unordered_map>
#include <string>
class MessageMap {
public:
	MessageMap() noexcept;
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const noexcept;
private:
	std::unordered_map<DWORD, std::string> map;
};