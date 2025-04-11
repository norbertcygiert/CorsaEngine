#define INCLUDE_ALL_WIN
#include "BetterWin.h"
#include "GDIPlus.h"
#include <gdiplus.h>
#include <algorithm>

ULONG_PTR GDIPlus::token = 0;
int GDIPlus::referenceCount = 0;
namespace Gdiplus {
	using std::min;
	using std::max;
};
namespace gdi = Gdiplus;
GDIPlus::GDIPlus() {
	if (referenceCount++ == 0) {
		gdi::GdiplusStartupInput(input);
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		gdi::GdiplusStartup(&token, &input, nullptr);
	}
}

GDIPlus::~GDIPlus(){
	if (--referenceCount == 0) {
		gdi::GdiplusShutdown(token);
	}
}