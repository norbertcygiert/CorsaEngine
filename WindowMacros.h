#pragma once
#define WND_EXCEPTION(cc) Window::HRESException(__LINE__, __FILE__, cc)
#define WND_LAST_EXCEPT() Window::HRESException(__LINE__,__FILE__, GetLastError())
#define WND_NOGRAPHICS_EXCEPT() Window::NoGraphicsException(__LINE__, __FILE__);