#include "main_menu.h"

#ifdef _WIN32
    #define PDC_FORCE_UTF8
    #define PDC_WIDE
    #define CHTYPE_64
    #include <curses.h>
#else
    #include <ncurses.h>
#endif

#include <cstring>


//  ____   __   ____    __   __   __ _  ____
// (  _ \ / _\ / ___) _(  ) / _\ (  ( \/ ___)
//  ) __//    \\___ \/ \) \/    \/    /\___ \
// (__)  \_/\_/(____/\____/\_/\_/\_)__)(____/


void MainMenu::Draw() {
    const char* ascii[] = {
        " ____   __   ____    __   __   __ _  ____",
        "(  _ \\ / _\\ / ___) _(  ) / _\\ (  ( \\/ ___)",
        " ) __//    \\\\___ \\/ \\) \\/    \\/    /\\___ \\",
        " (__)  \\_/\\_/(____/\\____/\\_/\\_/(_)__)(____/"
    };
    const int ascii_length = 4;

    int size_y, size_x;
    getmaxyx(stdscr, size_y, size_x);

    for (int index = 0; index < ascii_length; index++) 
    {
        const char* ascii_line = ascii[index];
        int line_len = strlen(ascii_line);
        move(index, (size_x - line_len) / 2);
        printw("%s", ascii_line);
    }
}

void MainMenu::Process() {}