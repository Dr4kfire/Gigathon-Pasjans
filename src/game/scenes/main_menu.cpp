#include "main_menu.h"

#ifdef _WIN32
#define PDC_FORCE_UTF8
#define PDC_WIDE
#define CHTYPE_64
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <cmath>
#include <cstring>

void MainMenu::Draw()
{
	int size_y, size_x;
	int position_x;
	getmaxyx(stdscr, size_y, size_x);

	// TITLE
	const char *ASCII[] = {
		" ____   __   __    __  ____  __   ____  ____ ",
		"/ ___) /  \\ (  )  (  )(_  _)/ _\\ (  _ \\(  __)",
		"\\___ \\(  O )/ (_/\\ )(   )( /    \\ )   / ) _) ",
		"(____/ \\__/ \\____/(__) (__)\\_/\\_/(__\\_)(____)",
		// " ____   __   ____    __   __   __ _  ____",
	    // "(  _ \\ / _\\ / ___) _(  ) / _\\ (  ( \\/ ___)",
	    // " ) __//    \\\\___ \\/ \\) \\/    \\/    /\\___ \\",
	    // "(__)  \\_/\\_/(____/\\____/\\_/\\_/(_)__)(____/"
	};
	const int ASCII_LENGTH = 4;

	position_x = std::round((size_x - strlen(ASCII[0])) / 2.0);

	for (int index = 0; index < ASCII_LENGTH; index++)
	{
		const char *ASCII_LINE = ASCII[index];
		int line_length = strlen(ASCII_LINE);

		move(index, position_x);
		printw("%s", ASCII_LINE);
	}

	// MENU BOX
	const int BOX_WIDTH = 24;
	position_x = std::round((size_x - BOX_WIDTH) / 2.0);

	const char* diff_opt_string = "EASY";
	if (m_hard_mode == true) {
		diff_opt_string = "HARD";
	}
	const char* ascii_comp_string = "FULL   ";
	if (m_full_ascii == false) {
		ascii_comp_string = "COMPACT";
	}

	mvprintw(9, position_x, "+----------------------+");
	mvprintw(10, position_x, "|                      |");
	switch (m_selected_option)
	{
	case START:
		attron(COLOR_PAIR(1));
		mvprintw(11, position_x, "| > Start              |");
		attroff(COLOR_PAIR(1));
		mvprintw(12, position_x, "|   Difficulty: %s   |", diff_opt_string);
		mvprintw(13, position_x, "|   ASCII: %s     |", ascii_comp_string);
		break;
	case DIFFICULTY:
		mvprintw(11, position_x, "|   Start              |");
		attron(COLOR_PAIR(1));
		mvprintw(12, position_x, "| > Difficulty: %s   |", diff_opt_string);
		attroff(COLOR_PAIR(1));
		mvprintw(13, position_x, "|   ASCII: %s     |", ascii_comp_string);
		break;
	case SETTINGS:
		mvprintw(11, position_x, "|   Start              |");
		mvprintw(12, position_x, "|   Difficulty: %s   |", diff_opt_string);
		attron(COLOR_PAIR(1));
		mvprintw(13, position_x, "| > ASCII: %s     |", ascii_comp_string);
		attroff(COLOR_PAIR(1));
		break;
	default:
		mvprintw(11, position_x, "|   Start              |");
		mvprintw(12, position_x, "|   Difficulty: %s   |", diff_opt_string);
		mvprintw(13, position_x, "|   ASCII: %s     |", ascii_comp_string);
		break;
	}
	mvprintw(14, position_x, "|                      |");
	mvprintw(15, position_x, "+----------------------+");

	// FULL ASCII WARNING
	if (m_full_ascii == true) {
		// const char* WARNING_TEXT[] = {
		// 	"* When using full ASCII mode please resize",
		// 	"  and scale your terminal window!"
		// };
		const char* WARNING_TEXT[] = {
			"* Currently not working!",
		};

		position_x = std::round((size_x - std::strlen(WARNING_TEXT[0])) / 2.0);
		attron(COLOR_PAIR(1));
		mvprintw(size_y - 4, position_x, "%s", WARNING_TEXT[0]);
		// mvprintw(size_y - 3, position_x, "%s", WARNING_TEXT[1]);
		attroff(COLOR_PAIR(1));
	}

	// FOOTER
	mvprintw(size_y - 1, 0, "Gigathon 2025");
	mvprintw(size_y - 1, size_x - 8, "Dr4kfire");
}

void MainMenu::Process(const int &input)
{
	switch (input)
	{
	case KEY_UP:
		option_idx--;
		if (option_idx < 0) {
			option_idx = 2;
		}
		break;
	case KEY_DOWN:
		option_idx++;
		if (option_idx >= 3) {
			option_idx = 0;
		}
		break;
	case '\n':
	case KEY_LEFT:
	case KEY_RIGHT:
		if (m_selected_option == Options::START) {
			SetChangeScene(true);
			result = {{"diff", m_hard_mode}, {"ascii", m_full_ascii}};
			break;
		}
		else if (m_selected_option == Options::DIFFICULTY) {
			m_hard_mode = !m_hard_mode;
			break;
		}

		m_full_ascii = !m_full_ascii;
		break;
	default:
		break;
	}

	m_selected_option = static_cast<Options>(option_idx);
}