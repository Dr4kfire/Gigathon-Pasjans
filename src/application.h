#pragma once 

#ifdef _WIN32
    #define PDC_FORCE_UTF8
    #define PDC_WIDE
    #define CHTYPE_64
    #include <curses.h>
#else
    #include <ncurses.h>
#endif


class Application {
public:
    Application();
    ~Application();
};