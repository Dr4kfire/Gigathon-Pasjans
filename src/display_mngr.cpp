#pragma once

#include "display_mngr.h"
#include <cstdlib>

void Display::DrawText() {
    
}

void Display::ClearConsole()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
