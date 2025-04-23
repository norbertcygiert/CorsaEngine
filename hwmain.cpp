#include "Application.h"

int __stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	try {
		return Application{}.start();
	}
	catch (const CorsaException& e) {
		MessageBox(nullptr, e.what(), e.getType(), MB_TASKMODAL | MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Lib Error", MB_OK | MB_ICONERROR);
	}
	catch (...) {
		MessageBox(nullptr, "No error message provided", "Unknown Error", MB_OK | MB_ICONERROR);
	}
	exit(0);
	return 0;
}