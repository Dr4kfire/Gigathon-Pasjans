#include "game_scene.h"
#include <algorithm>
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
const char SUIT_CHARS[] = {'H', 'D', 'C', 'S'};

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

	m_hovered_pos_x = position_x + (6 * m_cursor_x);
	m_hovered_pos_y = 4 + m_cursor_y;

	if (m_cursor_x == 0 && m_cursor_y == 0)
	{
		attron(COLOR_PAIR(1));
		DrawDeck(m_game_decks.additional, 4, position_x);
		attroff(COLOR_PAIR(1));
	}
	else
	{
		DrawDeck(m_game_decks.additional, 4, position_x);
	}

	DrawDeck(m_game_decks.draw_pile, 4, position_x + 6);

	DrawDeck(m_game_decks.sorted[0], 4, position_x + 18);
	DrawDeck(m_game_decks.sorted[1], 4, position_x + 18 + 6);
	DrawDeck(m_game_decks.sorted[2], 4, position_x + 18 + 12);
	DrawDeck(m_game_decks.sorted[3], 4, position_x + 18 + 18);

	int column_offset = 6;
	if (m_hard_mode)
	{
		column_offset = 9;
	}

	for (int i = 0; i < 7; i++)
	{
		DrawDeck(m_game_decks.columns[i], column_offset, position_x + (6 * i));
	}

	// SELECT CURSORS
	attron(COLOR_PAIR(1));
	mvprintw(m_hovered_pos_y, 0, ">");

	mvprintw(3, m_hovered_pos_x + 2, "\\");
	attroff(COLOR_PAIR(1));
}

void GameScene::Process(const int &input)
{
	// ADDITIONAL DECK
	if (m_cursor_x == 0 && m_cursor_y == 0)
	{
		m_hovered_deck = &m_game_decks.additional;
	}
	// SORTED DECKS
	else if (m_cursor_x > 2 && m_cursor_y == 0)
	{
		m_hovered_deck = &m_game_decks.sorted[m_cursor_x - 2];
	}
	else if (!m_hard_mode)
	{
		// DRAW PILE
		if (m_cursor_x == 1 && m_cursor_y == 0)
		{
			m_hovered_deck = &m_game_decks.draw_pile;
		}
		// COLUMN DECKS
		else if (m_cursor_y > 1)
		{
			m_hovered_deck = &m_game_decks.columns[m_cursor_x];
		}
	}
	else
	{
		// DRAW PILE
		if (m_cursor_x == 1 && m_cursor_y == 2)
		{
			m_hovered_deck = &m_game_decks.draw_pile;
		}
		// COLUMN DECKS
		else if (m_cursor_y > 2)
		{
			m_hovered_deck = &m_game_decks.columns[m_cursor_x];
		}
	}

	switch (input)
	{
	case KEY_UP:
		m_cursor_y = std::max(0, m_cursor_y - 1);
		if (m_cursor_y == 1)
			m_cursor_y = 0;

		break;
	case KEY_DOWN:
		m_cursor_y++;
		if (m_hard_mode)
		{
			if (m_cursor_x == 1 && (m_cursor_y == 0 || m_cursor_y == 1))
			{
				m_cursor_y = 2;
			}
			break;
		}


		if (m_hard_mode) {

		} 
		else {
			if (m_cursor_y == 1)
			{
				m_cursor_y = 2;
			}
			else if (m_cursor_y > m_game_decks.columns[m_cursor_x].GetSize() + 1 && m_game_decks.columns[m_cursor_x].GetSize() > 0)
			{
				m_cursor_y = m_game_decks.columns[m_cursor_x].GetSize() + 1;
			}
			else if (m_cursor_y >= m_game_decks.columns[m_cursor_x].GetSize() + 2) {
				m_cursor_y = 2;
			}
		}
		
		break;

	case KEY_LEFT:
		m_cursor_x = std::max(0, m_cursor_x - 1);

		if (m_hard_mode) {

		} 
		else {
			if (m_cursor_y == 1)
			{
				m_cursor_y = 2;
			}
			else if (m_cursor_y > m_game_decks.columns[m_cursor_x].GetSize() + 1 && m_game_decks.columns[m_cursor_x].GetSize() > 0)
			{
				m_cursor_y = m_game_decks.columns[m_cursor_x].GetSize() + 1;
			}
			else if (m_cursor_y >= m_game_decks.columns[m_cursor_x].GetSize() + 2) {
				m_cursor_y = 2;
			}
		}
		break;
	case KEY_RIGHT:
		m_cursor_x = std::clamp(m_cursor_x + 1, 0, 6);

		// SKIP THE FREE SPACE
		if (m_hard_mode) {

		} 
		else {
			if (m_cursor_y == 1)
			{
				m_cursor_y = 2;
			}
			else if (m_cursor_y > m_game_decks.columns[m_cursor_x].GetSize() + 1 && m_game_decks.columns[m_cursor_x].GetSize() > 0)
			{
				m_cursor_y = m_game_decks.columns[m_cursor_x].GetSize() + 1;
			}
			else if (m_cursor_y >= m_game_decks.columns[m_cursor_x].GetSize() + 2) {
				m_cursor_y = 2;
			}
		}
		break;

	case '\n':
	case KEY_ENTER:
		if (m_cursor_x == 0 && m_cursor_y == 0)
		{
			if (m_hard_mode)
			{
				DrawCardsFromAdditional(3);
			}
			else
			{
				DrawCardsFromAdditional(1);
			}
			break;
		}

		// NOTHING SELECTED
		if (m_selected_pos_x == -1 && m_selected_pos_y == -1)
		{
			m_selected_pos_x = m_hovered_pos_x;
			m_selected_pos_y = m_hovered_pos_y;
			m_selected_deck = m_hovered_deck;
			if (m_hovered_deck == &m_game_decks.draw_pile) 
			{
				m_selected_card_index = 0;
			}
			else if (m_hard_mode)
			{
				m_selected_card_index = m_cursor_y - 4;
			}
			else
			{
				m_selected_card_index = m_cursor_y - 2;
			}
		}
		// SELECTED SOMETHING THAT ALREADY IS SELECTED
		else if (m_selected_pos_y == m_hovered_pos_y && m_selected_pos_x == m_hovered_pos_x)
		{
			m_selected_pos_y = -1;
			m_selected_pos_x = -1;
			m_selected_deck = nullptr;
			m_selected_card_index = -1;
		}
		// SELECTED SOMETHING THAT ISN'T SELECTED - CHECK IF CAN BE MOVED
		else if (CanRepositionCard(*m_hovered_deck, m_selected_deck->GetCardReference(m_selected_card_index)))
		{
			bool only_one_card = false;
			if (m_selected_deck == &m_game_decks.draw_pile) {
				only_one_card = true;
			}
			else if (m_selected_deck->sort_deck == true) {
				only_one_card = true;
			}

			RepositionCards(*m_selected_deck, *m_hovered_deck, m_selected_card_index, only_one_card);
			m_selected_pos_y = -1;
			m_selected_pos_x = -1;
			m_selected_deck = nullptr;
			m_selected_card_index = -1;
		}
		// DISSELECT THE OLD ONE AND SELECT A NEW ONE
		else
		{
			m_selected_pos_x = m_hovered_pos_x;
			m_selected_pos_y = m_hovered_pos_y;
			m_selected_deck = m_hovered_deck;
			if (m_hard_mode)
			{
				m_selected_card_index = m_cursor_y - 4;
			}
			else
			{
				m_selected_card_index = m_cursor_y - 2;
			}
		}
		break;
	default:
		break;
	}
}

string GameScene::GetCardString(const Card &card)
{
	char suit_char = SUIT_CHARS[card.suit];
	string card_str = "";

	if (!m_full_ascii)
	{
		if (card.hidden)
		{
			return "[? ?]";
		}
		if (card.rank >= 10)
		{
			return "[" + std::to_string(card.rank) + suit_char + "]";
		}
		return "[" + std::to_string(card.rank) + " " + suit_char + "]";
	}

	return "";
}

void GameScene::DrawCard(int card_index, Deck &deck, int pos_y, int pos_x)
{
	const Card &card = deck.GetConstCardReference(card_index);

	char suit_char = SUIT_CHARS[card.suit];
	string card_str = GetCardString(card);

	if (pos_x == m_selected_pos_x && pos_y == m_selected_pos_y && !card.hidden)
	{
		attron(COLOR_PAIR(3));
	}
	else if (pos_x == m_hovered_pos_x && pos_y == m_hovered_pos_y)
	{
		attron(COLOR_PAIR(1));
	}
	else if ((card.suit == Card::HEARTS || card.suit == Card::DIAMONDS) && !card.hidden)
	{
		attron(COLOR_PAIR(2));
	}

	mvprintw(pos_y, pos_x, "%s", card_str.c_str());

	attroff(COLOR_PAIR(1));
	attroff(COLOR_PAIR(2));
	attroff(COLOR_PAIR(3));
}

void GameScene::DrawDeck(Deck &deck, int pos_y, int pos_x)
{
	if (deck.GetSize() == 0)
	{
		if (pos_y == m_hovered_pos_y && pos_x == m_hovered_pos_x)
		{
			attron(COLOR_PAIR(1));
		}
		mvprintw(pos_y, pos_x, COMPACT_TEMPLATE, "  ", " ");
		attroff(COLOR_PAIR(1));
		return;
	}

	if (deck.hidden)
	{
		if (pos_y == m_hovered_pos_y && pos_x == m_hovered_pos_x)
		{
			attron(COLOR_PAIR(1));
		}
		mvprintw(pos_y, pos_x, COMPACT_TEMPLATE, "? ", "?");
		attroff(COLOR_PAIR(1));
		return;
	}

	if (!deck.draw_as_column)
	{
		DrawCard(deck.GetSize() - 1, deck, pos_y, pos_x);
		return;
	}

	int offset = 0;
	int first_index =
		std::clamp((int)(deck.GetSize() - deck.max_column_size), 0, (int)(deck.GetSize() - 1));

	for (int card_index = first_index; card_index < deck.GetSize(); card_index++)
	{
		DrawCard(card_index, deck, pos_y + offset, pos_x);
		offset++;
	}
}

void GameScene::DrawCardsFromAdditional(int ammount)
{
	if (m_game_decks.additional.GetSize() == 0)
	{
		ReplenishCardsToAdditional();
		return;
	}

	if (m_game_decks.additional.GetSize() - ammount < 0)
	{
		for (int i = 0; i < m_game_decks.additional.GetSize(); i++)
		{
			Card new_card = m_game_decks.additional.PopFrontCard();
			m_game_decks.draw_pile.AppendCard(std::move(new_card), false);
		}
		return;
	}
	for (int i = 0; i < ammount; i++)
	{
		Card new_card = m_game_decks.additional.PopFrontCard();
		m_game_decks.draw_pile.AppendCard(std::move(new_card), false);
	}
}

void GameScene::ReplenishCardsToAdditional()
{
	for (int i = m_game_decks.draw_pile.GetSize() - 1; i > 0; i--)
	{
		Card new_card = m_game_decks.draw_pile.PopAt(i);
		m_game_decks.additional.AppendCard(std::move(new_card), true);
	}
}

bool GameScene::CanRepositionCard(Deck &new_deck, Card &card)
{
	// IF THERE IS NO TOP CARD THE CARD MUST BE A KING
	if (new_deck.GetSize() == 0 && card.rank == 12)
	{
		return true;
	}
	else if (new_deck.sort_deck == true) {
		if (new_deck.GetSize() == 0 && card.rank == 0) {
			return true;
		}
		
		const Card &top_card = new_deck.GetConstCardReference(new_deck.GetSize() - 1);
		if (top_card.suit != card.suit) {
			return false;
		}
		if (top_card.rank + 1 != card.suit) {
			return false;
		}

		return true;
	}

	const Card &top_card = new_deck.GetConstCardReference(new_deck.GetSize() - 1);

	// CHECK IF THE CARD IS HIDDEN
	if (top_card.hidden)
	{
		return false;
	}

	// CHECK IF THE SUIT MATCHES
	if (top_card.suit == Card::CLUBS || top_card.suit == Card::SPADES)
	{
		if (card.suit == Card::CLUBS || card.suit == Card::SPADES)
		{
			return false;
		}
	}
	else
	{
		if (card.suit == Card::HEARTS || card.suit == Card::DIAMONDS)
		{
			return false;
		}
	}

	// CHECK IF THE RANK MATCHES
	if (top_card.rank - 1 != card.rank)
	{
		return false;
	}

	// OTHERWISE: THE CARD MATCHES AND CAN BE PLACED
	return true;
}

void GameScene::RepositionCards(Deck &original_deck, Deck &new_deck, int first_card_index, bool only_one_card)
{
	if (only_one_card) {
		Card repositioned_card = original_deck.PopFrontCard();
		new_deck.AppendCard(std::move(repositioned_card), false);
		original_deck.GetCardReference(original_deck.GetSize() - 1).hidden = false;
		return;
	}
	for (int card_idx = first_card_index; card_idx < original_deck.GetSize(); card_idx++)
	{
		Card repositioned_card = original_deck.PopAt(card_idx);
		new_deck.AppendCard(std::move(repositioned_card), false);
	}
	original_deck.GetCardReference(original_deck.GetSize() - 1).hidden = false;
}
