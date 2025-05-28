#include "game_scene.h"
#include <cstddef>
#include <cstdio>
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

const char *FULL_TEMPLATE[] = {
    ".-------.", 
	"|%2s   %s|", 
	"|       |", 
	"|   %s   |",
    "|       |", 
	"|     %2s|", 
	"`-------'",
};
const char *COMPACT_TEMPLATE = "[%2s%s]";
const char SUIT_CHARS[] = { 'H', 'D', 'C', 'S' };

void GameScene::Draw()
{
	int size_y, size_x;
	int position_x;
	getmaxyx(stdscr, size_y, size_x);

	// TOP BAR
	for (int i = 0; i < size_x; i++)
	{
		move(0, i);
		printw("-");
	}

	const char *SUITS_INFO = "[H]EARTS, [D]IAMONDS, [C]LUBS, [S]PADES";

	position_x = std::round((size_x - strlen(SUITS_INFO)) / 2.0);
	mvprintw(1, position_x, "%s", SUITS_INFO);

	for (int i = 0; i < size_x; i++)
	{
		move(0, i);
		printw("-");
	}

	// ADDITIONAL DECK, DRAW PILE & SORTED PILES
	char COMPACT_ROW_TEXT[42] = "\0";
	position_x = std::round((size_x - strlen(COMPACT_ROW_TEXT)) / 2.0);

	snprintf(COMPACT_ROW_TEXT, sizeof(COMPACT_ROW_TEXT), COMPACT_TEMPLATE, "? ", "?");
	strcat(COMPACT_ROW_TEXT, " ");
	snprintf(COMPACT_ROW_TEXT, sizeof(COMPACT_ROW_TEXT), COMPACT_TEMPLATE, "? ", "?");

	

	// if (m_game_decks.additional.GetSize() > 0)
	// {
	// 	mvprintw(4, position_x, COMPACT_TEMPLATE, "? ", "?");
	// }
	// else
	// {
	// 	mvprintw(4, position_x, const char *, ...)
	// }
}

void GameScene::Process(const int &input) {}

void GameScene::GetCardDrawBuffer(Card& card, char *buffer, size_t buffer_size)
{
	char suit_char = SUIT_CHARS[card.suit];

	if (!m_full_ascii) {
		if (card.hidden) {
			snprintf(buffer, buffer_size, COMPACT_TEMPLATE, "? ", "?");
			return;
		}
		snprintf(buffer, buffer_size, COMPACT_TEMPLATE, card.rank, suit_char);
		return;
	}

	for (int i = 0; i < 7; i++) {
		if (i % 2 == 0) {
			snprintf(buffer, buffer_size, "%s", FULL_TEMPLATE[i]);
			continue;
		}

		switch (i) {
		case 1:
			snprintf(buffer, buffer_size, FULL_TEMPLATE[i], card.rank, suit_char);
			break;
		case 3:
			snprintf(buffer, buffer_size, FULL_TEMPLATE[i], suit_char);
			break;
		case 7:
			snprintf(buffer, buffer_size, FULL_TEMPLATE[i], card.rank);
			break;
		default:
			snprintf(buffer, buffer_size, "%s", FULL_TEMPLATE[i]);
			break;
		}
	}
}

void GameScene::GetDeckDrawBuffer(Deck &deck, char *buffer, size_t buffer_size)
{
	if (deck.GetSize() == 0) {
		snprintf(buffer, buffer_size, COMPACT_TEMPLATE, "? ", "?");
		return;
	}

	const Card& top_card = deck.GetTopCard();
	// GetCardDrawBuffer(top_card, buffer, buffer_size);
}