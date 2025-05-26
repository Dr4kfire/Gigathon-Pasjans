#include "game_scene.h"
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



void GameScene::Draw() {
	int size_y, size_x;
	int position_x;
	getmaxyx(stdscr, size_y, size_x);

	// TOP BAR
	for (int i = 0; i < size_x; i++) {
		move(0, i);
		printw("-");
	}

	const char* SUITS_INFO = "[H]EARTS, [D]IAMONDS, [C]LUBS, [S]PADES";
	
	position_x = std::round((size_x - std::strlen(SUITS_INFO)) / 2.0);
	mvprintw(1, position_x, "%s", SUITS_INFO);

	for (int i = 0; i < size_x; i++) {
		move(0, i);
		printw("-");
	}

	// ADDITIONAL DECK, DRAW PILE & SORTED PILES
	
}

void GameScene::Process(const int& input) {

}