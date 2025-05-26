#ifdef _WIN32
    #define PDC_FORCE_UTF8
    #define PDC_WIDE
    #define CHTYPE_64
    #include <curses.h>
#else
    #include <ncurses.h>
#endif

#include "application.h"

int main(int argc, char const *argv[])
{
    Application app;

    while (app.IsRunning())
    {
        app.Draw();

        app.Process();
    }
}
