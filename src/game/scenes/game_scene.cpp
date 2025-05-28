#include "game_scene.h"
#include <cstddef>
#include <cstdio>
#include <string>
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

using std::string;


const char* FULL_TEMPLATE[] = {
    ".-------.", 
	"|%2s   %s|", 
	"|       |", 
	"|   %s   |",
    "|       |", 
	"|     %2s|", 
	"`-------'",
};
const char* COMPACT_TEMPLATE = "[%2s%s]";
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

	const string SUITS_INFO = "[H]EARTS, [D]IAMONDS, [C]LUBS, [S]PADES";

	position_x = std::round((size_x - SUITS_INFO.length()) / 2.0);
	mvprintw(1, position_x, "%s", SUITS_INFO.c_str());

	for (int i = 0; i < size_x; i++)
	{
		move(2, i);
		printw("-");
	}

	// ADDITIONAL DECK, DRAW PILE & SORTED PILES
	position_x = std::ceil((size_x - 41) / 2.0);

	m_highlighted_pos_x = position_x + (6 * m_cursor_x);
	m_highlighted_pos_y = 4 + m_cursor_y;

	if (m_cursor_x == 0 && m_cursor_y == 0) {
		attron(COLOR_PAIR(1));
		DrawDeck(m_game_decks.additional, 4, position_x);
		attroff(COLOR_PAIR(1));
	} else {
		DrawDeck(m_game_decks.additional, 4, position_x);
	}
	
	DrawDeck(m_game_decks.draw_pile, 4, position_x + 6);

	DrawDeck(m_game_decks.sorted[0], 4, position_x + 18);
	DrawDeck(m_game_decks.sorted[1], 4, position_x + 18 + 6);
	DrawDeck(m_game_decks.sorted[2], 4, position_x + 18 + 12);
	DrawDeck(m_game_decks.sorted[3], 4, position_x + 18 + 18);

	int column_offset = 6;
	if (m_hard_mode) {
		column_offset = 9;
	}

	for (int i = 0; i < 7; i++) {
		DrawDeck(m_game_decks.columns[i], column_offset, position_x + (6*i));
	}

	// SELECT CURSORS
	attron(COLOR_PAIR(1));
	mvprintw(m_highlighted_pos_y, 0, ">");

	mvprintw(3, m_highlighted_pos_x + 2, "\\");
	attroff(COLOR_PAIR(1));
}


void GameScene::Process(const int &input) 
{
	switch (input)
	{
	case KEY_UP:
		m_cursor_y--;
		if (m_cursor_y < 0) {
			m_cursor_y = 0;
		}
		if (m_cursor_y == 1) {
			m_cursor_y = 0;
		}
		break;
	case KEY_DOWN:
		m_cursor_y++;
		if (m_hard_mode) {
			if (m_cursor_x == 1 && (m_cursor_y == 0 || m_cursor_y == 1)) {
				m_cursor_y = 2;
			}
			break;
		}

		if (m_cursor_y == 1) {
			m_cursor_y = 2;
		} 
		else if (m_cursor_y > m_game_decks.columns[m_cursor_x].GetSize()+1) {
			m_cursor_y = m_game_decks.columns[m_cursor_x].GetSize()+1;
		}
		break;

	case KEY_LEFT:
		m_cursor_x--;
		if (m_cursor_x < 0) {
			m_cursor_x = 0;
		} else if (m_cursor_x == 2 && m_cursor_y == 0) {
			m_cursor_x = 1;
		}
		else if (m_cursor_y > m_game_decks.columns[m_cursor_x].GetSize()+1) {
			m_cursor_y = m_game_decks.columns[m_cursor_x].GetSize()+1;
		}
		break;
	case KEY_RIGHT:
		m_cursor_x++;
		if (m_cursor_x >= 7) {
			m_cursor_x = 6;
		} else if (m_cursor_x == 2 && m_cursor_y == 0) {
			m_cursor_x = 3;
		}
		else if (m_cursor_y > m_game_decks.columns[m_cursor_x].GetSize()+1) {
			m_cursor_y = m_game_decks.columns[m_cursor_x].GetSize()+1;
		}
		break;
	case '\n':
	case KEY_ENTER:
		if (m_selected_pos_y == m_highlighted_pos_y && m_selected_pos_x == m_highlighted_pos_x) {
			m_selected_pos_y = -1;
			m_selected_pos_x = -1;
		} else {
			m_selected_pos_x = m_highlighted_pos_x;
			m_selected_pos_y = m_highlighted_pos_y;
		}
	default:
		break;
	}
}


string GameScene::GetCardString(const Card& card)
{
	char suit_char = SUIT_CHARS[card.suit];
	string card_str = "";

	if (!m_full_ascii) {
		if (card.hidden) {
			return "[? ?]";
		}
		if (card.rank >= 10) {
			return "[" + std::to_string(card.rank) + suit_char + "]";
		}
		return "[" + std::to_string(card.rank) + " " + suit_char + "]";
	}

	return "";
}

void GameScene::DrawCard(const Card& card, int pos_y, int pos_x) {
	char suit_char = SUIT_CHARS[card.suit];
	string card_str = "";

	if (!m_full_ascii) {
		std::string card_str = GetCardString(card);
		if (pos_y == m_selected_pos_y && pos_x == m_selected_pos_x) {
			m_selected_card = std::make_unique<Card>(card);
			attron(COLOR_PAIR(3));
    		mvprintw(pos_y, pos_x, "%s", card_str.c_str());
			attroff(COLOR_PAIR(3));
			return;
		}
		else if (pos_y == m_highlighted_pos_y && pos_x == m_highlighted_pos_x) {
			attron(COLOR_PAIR(1));
    		mvprintw(pos_y, pos_x, "%s", card_str.c_str());
			attroff(COLOR_PAIR(1));
			return;
		}
		
    	mvprintw(pos_y, pos_x, "%s", card_str.c_str());
	}
}


void GameScene::DrawDeck(Deck& deck, int pos_y, int pos_x) {
	if (!m_full_ascii) {
		if (deck.GetSize() == 0) {
			if (pos_y == m_selected_pos_y && pos_x == m_selected_pos_x) {
				m_selected_pos_y = -1;
				m_selected_pos_x = -1;
			}
			if (pos_y == m_highlighted_pos_y && pos_x == m_highlighted_pos_x) {
				attron(COLOR_PAIR(1));
				mvprintw(pos_y, pos_x, COMPACT_TEMPLATE, "  ", " ");
				attroff(COLOR_PAIR(1));
				return;
			}
			mvprintw(pos_y, pos_x, COMPACT_TEMPLATE, "  ", " ");
			return;
		}
		if (deck.hidden) {
			if (pos_y == m_selected_pos_y && pos_x == m_selected_pos_x) {
				if (m_cursor_x == 0 && m_cursor_y == 0) {
					
					m_selected_pos_y = -1;
					m_selected_pos_x = -1;
				}
			}
			else if (pos_y == m_highlighted_pos_y && pos_x == m_highlighted_pos_x) {
				attron(COLOR_PAIR(1));
				mvprintw(pos_y, pos_x, COMPACT_TEMPLATE, "? ", "?");
				attroff(COLOR_PAIR(1));
				return;
			}
			mvprintw(pos_y, pos_x, COMPACT_TEMPLATE, "? ", "?");
			return;
		}
		if (!deck.draw_as_column) {
			const Card& top_card = deck.GetConstCardReference(deck.GetSize()-1);

			if (top_card.suit == Card::HEARTS || top_card.suit == Card::DIAMONDS) {
				attron(COLOR_PAIR(2));
				DrawCard(top_card, pos_y, pos_x);
				attroff(COLOR_PAIR(2));
				return;
			}
			DrawCard(top_card, pos_y, pos_x);	
		}

		int first_index = (int)(deck.GetSize()) - deck.max_column_size;
		
		if (first_index < 0)
			first_index = 0;

		int offset = 0;
		for (int i = first_index; i < deck.GetSize(); i++) 
		{
			const Card& card = deck.GetConstCardReference(i);
			string card_text = GetCardString(card);

			if (card.hidden == false && (card.suit == Card::HEARTS || card.suit == Card::DIAMONDS)) {
				attron(COLOR_PAIR(2));
				DrawCard(card, pos_y + offset, pos_x);
				attroff(COLOR_PAIR(2));
			} else {
				DrawCard(card, pos_y + offset, pos_x);
			}
			offset++;
		}
	}
}