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
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);

	if (has_colors())
	{
		start_color();
		init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
	}
}
Application::~Application() { endwin(); }

void Application::Process()
{
	int input = getch();

	game.Process(input);
}

void Application::Draw()
{
	clear();
	game.Draw();
	refresh();
}

void Application::Close() { m_is_running = false; }

bool Application::IsRunning() { return m_is_running; }
