#pragma once
#include "BetterWin.h"
#include "Exceptions.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include <optional>
//https://en.wikipedia.org/wiki/Singleton_pattern for future reference
class Window {
private:
	int w, h;
	HWND hWnd;
public:
	KeyboardHandler keybd;
	MouseHandler mouse;

	Window(int w, int h, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages();


	static LRESULT __stdcall HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

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
	

	class WndClass 
	{
	public:
		static const char* getName() noexcept; //lowercase first letter so i can see wheter its mine or winapi
		static HINSTANCE getInstance() noexcept;
	private:
		WndClass() noexcept;
		~WndClass();
		WndClass(const WndClass&) = delete;
		WndClass& operator=(const WndClass&) = delete;
		static constexpr const char* wndClassName = "EngineWnd";
		static WndClass wndClass;
		HINSTANCE hInstance;
	};
};
//helper macros

#define WND_EXCEPTION(hr) Window::HRESException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() Window::HRESException(__LINE__,__FILE__, GetLastError())
