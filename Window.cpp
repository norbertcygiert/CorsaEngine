#include "Window.h"
#include <sstream>
Window::WndClass Window::WndClass::wndClass;

Window::WndClass::WndClass() noexcept : hInstance(GetModuleHandle(nullptr)) {
	//Creating and Registering a WindowClass
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::~Window() { DestroyWindow(hWnd); }
Window::WndClass::~WndClass() { UnregisterClass(wndClassName, getInstance()); }
const char* Window::WndClass::getName() noexcept{ return wndClassName; }
HINSTANCE Window::WndClass::getInstance() noexcept { return wndClass.hInstance; }

Window::Window(int w, int h, const char* name) : w(w),h(h) {
	RECT wr;
	wr.left = 100;
	wr.right = w + wr.left;
	wr.top = 100;
	wr.bottom = h + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false) == 0) {
		throw WND_LAST_EXCEPT();
	}
	hWnd = CreateWindow(
		WndClass::getName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr,
		WndClass::getInstance(),
		this
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

std::optional<int> Window::ProcessMessages() {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

LRESULT __stdcall Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	//Insane evil hack to use OOP on WinApi created windows.
	if (msg == WM_NCCREATE) {
		//https://en.cppreference.com/w/cpp/language/reinterpret_cast
		//Extract the window pointer from created struct.
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//Save the extracted pointer to GWLP_USERDATA (so now our window class is what is actualy on the WinApi side)
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//Swap the message handling procedure to our own
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		//Forward message to our handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

LRESULT __stdcall Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	//Get pointer to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		//Check the 30th bit for if the key was being held before
		//https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-syskeydown#parameters
		if (!(lParam & 0x40000000) || keybd.autorepeatOn()) {
			keybd.onKeyDown(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_SYSKEYDOWN: //For handling ALT-key and other systemkeys
		if (!(lParam & 0x40000000) || keybd.autorepeatOn()) { 
			keybd.onKeyDown(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		keybd.onKeyUp(static_cast<unsigned char>(wParam));
		break;
	case WM_SYSKEYUP:
		keybd.onKeyUp(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keybd.onChar(static_cast<unsigned char>(wParam));
		break;
	case WM_KILLFOCUS:
		keybd.clearState();
		break;
	case WM_MOUSEMOVE:
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x > 0 && pt.x < w && pt.y > 0 && pt.y < h) {
			mouse.onMove(pt.x, pt.y);
			if (!mouse.isInWindow()) {
				SetCapture(hWnd);
				mouse.onWindowEnter();
			}
		}
		else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.onMove(pt.x, pt.y);
			}
			else {
				ReleaseCapture();
				mouse.onWindowLeave();
			}
		}
		break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

//                                       EXCEPTIONS


std::string Window::Exception::translateErrorCode(HRESULT hr) noexcept{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		//failure
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}



const char* Window::HRESException::what() const noexcept{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error Code] " << getErrorCode() << std::endl
		<< "[Description] " << getErrorDescription() << std::endl
		<< getOriginString();
	buf = oss.str();
	return buf.c_str();
}


Window::HRESException::HRESException(int line, const char* file, HRESULT hr) noexcept : Exception(line, file), hr(hr) {}
const char* Window::HRESException::getType() const noexcept { return "Window HrException"; }
HRESULT Window::HRESException::getErrorCode() const noexcept { return hr; }
std::string Window::HRESException::getErrorDescription() const noexcept { return Exception::translateErrorCode(hr); }

