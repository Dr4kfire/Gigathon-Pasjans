#include "application.h"

#ifdef _WIN32
    #define PDC_FORCE_UTF8
    #define PDC_WIDE
    #define CHTYPE_64
    #include <curses.h>
#else
    #include <ncurses.h>
#endif


Application::Application() 
{
    initscr();
    noecho(); cbreak(); curs_set(0); keypad(stdscr, TRUE);
}
Application::~Application() 
{
    endwin();
}


void Application::Process() 
{
    int ch = getch();
    if (ch == KEY_RESIZE) {
        
    }
}

void Application::Draw() 
{
    clear();
    game.Draw();
    refresh();
}


void Application::Close() 
{
    m_is_running = false;
}

bool Application::IsRunning() 
{
    return m_is_running;
}

