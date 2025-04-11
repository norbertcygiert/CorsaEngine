#pragma once
#include "BetterWin.h"

class GDIPlus {
private:
	static ULONG_PTR token;
	static int referenceCount;
public:
	GDIPlus();
	~GDIPlus();
};