#include "card.h"

// INCLUDE NCURSES (Unix) OR PDCURSES (Windows)
#ifdef _WIN32
#define PDC_FORCE_UTF8
#define PDC_WIDE
#define CHTYPE_64
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <string>
using std::string;


const char* Card::CARD_TEMPLATE = "[%2s%s]";
const char* Card::CARD_HIDDEN = "` ? `";

const char Card::SUIT_CHARS[4] = {'H', 'D', 'C', 'S'};
const std::string Card::RANK_STRINGS[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};


void Card::Draw(int pos_y, int pos_x) const
{	
	// Get the card's string
	string card_str = "";

	char suit_char = SUIT_CHARS[card_data.suit];
	string rank_str = RANK_STRINGS[card_data.rank];

	if (card_data.hidden)
	{
		card_str = CARD_HIDDEN;
	}
	else {
		card_str = "[" + rank_str + " " + suit_char + "]";
		
		if (rank_str == "10") card_str = "[" + rank_str + suit_char + "]";
	}


	// Draw the card using correct colors
	if (m_selected && !card_data.hidden)
	{
		attron(COLOR_PAIR(3));
	}
	else if (m_hovered)
	{
		attron(COLOR_PAIR(1));
	}
	else if ((card_data.suit == CardData::HEARTS || card_data.suit == CardData::DIAMONDS) && !card_data.hidden)
	{
		attron(COLOR_PAIR(2));
	}

	mvprintw(pos_y, pos_x, "%s", card_str.c_str());

	attroff(COLOR_PAIR(1));
	attroff(COLOR_PAIR(2));
	attroff(COLOR_PAIR(3));
}